// ウィンドウ関連の処理
#include "GgApplication.h"

// シェーダー関連の処理
#include "shader.h"

// オブジェクト関連の処理
#include "object.h"

//
// 単位行列を設定する
//
//   m: 単位行列を格納する配列
//
static void loadIdentity(GLfloat *m)
{
  m[ 0] = m[ 5] = m[10] = m[15] = 1.0f;
  m[ 1] = m[ 2] = m[ 3] = m[ 4] =
  m[ 6] = m[ 7] = m[ 8] = m[ 9] =
  m[11] = m[12] = m[13] = m[14] = 0.0f;
}

//
// 直交投影変換行列を求める
//
//   m: 直交投影変換行列を格納する配列
//   left, right: ビューボリュームの左右端
//   bottom, top: ビューボリュームの上下端
//   zNear, zFar: 前方面および後方面までの距離
//
static void ortho(GLfloat *m, float left, float right, float bottom, float top, float zNear, float zFar)
{
  m[ 0] =  2.0f / (right - left);
  m[ 5] =  2.0f / (top - bottom);
  m[10] = -2.0f / (zFar - zNear);
  m[12] = -(right + left) / (right - left);
  m[13] = -(top + bottom) / (top - bottom);
  m[14] = -(zFar + zNear) / (zFar - zNear);
  m[15] =  1.0f;
  m[ 1] = m[ 2] = m[ 3] = m[ 4] = m[ 6] = m[ 7] = m[ 8] = m[ 9] = m[11] = 0.0f;
}

//
// 透視投影変換行列を求める
//
//   m: 透視投影変換行列を格納する配列
//   left, right: 前方面の左右端
//   bottom, top: 前方面の上下端
//   zNear, zFar: 前方面および後方面までの距離
//
static void frustum(GLfloat *m, float left, float right, float bottom, float top, float zNear, float zFar)
{
  m[ 0] =  2.0f * zNear / (right - left);
  m[ 5] =  2.0f * zNear / (top - bottom);
  m[ 8] =  (right + left) / (right - left);
  m[ 9] =  (top + bottom) / (top - bottom);
  m[10] = -(zFar + zNear) / (zFar - zNear);
  m[11] = -1.0f;
  m[14] = -2.0f * zFar * zNear / (zFar - zNear);
  m[ 1] = m[ 2] = m[ 3] = m[ 4] = m[ 6] = m[ 7] = m[12] = m[13] = m[15] = 0.0f;
}

//
// 画角を指定して透視投影変換行列を求める
//
//   m: 透視投影変換行列を格納する配列
//   fovy: 画角（ラジアン）
//   aspect: ウィンドウの縦横比
//   zNear, zFar: 前方面および後方面までの距離
//
static void perspective(GLfloat *m, float fovy, float aspect, float zNear, float zFar)
{
  // 【宿題】ここを解答してください（loadIdentity() を置き換えてください）
  loadIdentity(m);
}

//
// ビュー変換行列を求める
//
//   m: ビュー変換行列を格納する配列
//   ex, ey, ez: 視点の位置
//   tx, ty, tz: 目標点の位置
//   ux, uy, uz: 上方向のベクトル
//
static void lookat(GLfloat *m, float ex, float ey, float ez, float tx, float ty, float tz, float ux, float uy, float uz)
{
  // 【宿題】ここを解答してください（loadIdentity() を置き換えてください）
  loadIdentity(m);
}

//
// 4 行 4 列の行列の積を求める
//
//   m ← m1 × m2
//
static void multiply(GLfloat *m, const GLfloat *m1, const GLfloat *m2)
{
  for (int i = 0; i < 16; ++i)
  {
    int j = i & 3, k = i & ~3;

    // 配列変数に行列が転置された状態で格納されていることを考慮している
    m[i] = m1[0 + j] * m2[k + 0] + m1[4 + j] * m2[k + 1] + m1[8 + j] * m2[k + 2] + m1[12 + j] * m2[k + 3];
  }
}

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("ggsample03");

  // 背景色を指定する
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // プログラムオブジェクトの作成
  const GLuint program(loadProgram("ggsample03.vert", "pv", "ggsample03.frag", "fc"));

  // uniform 変数のインデックスの検索（見つからなければ -1）
  const GLint mcLoc(glGetUniformLocation(program, "mc"));

  // ビュー変換行列を mv に求める
  GLfloat mv[16];
  lookat(mv, 3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  // 頂点属性
  static constexpr GLfloat position[][3] =
  {
    { -0.9f, -0.9f, -0.9f },  // (0)
    {  0.9f, -0.9f, -0.9f },  // (1)
    {  0.9f, -0.9f,  0.9f },  // (2)
    { -0.9f, -0.9f,  0.9f },  // (3)
    { -0.9f,  0.9f, -0.9f },  // (4)
    {  0.9f,  0.9f, -0.9f },  // (5)
    {  0.9f,  0.9f,  0.9f },  // (6)
    { -0.9f,  0.9f,  0.9f },  // (7)
  };
  static constexpr int vertices(sizeof position / sizeof position[0]);

  // 頂点インデックス
  static constexpr GLuint index[] =
  {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    0, 4,
    1, 5,
    2, 6,
    3, 7,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
  };
  static constexpr GLuint lines(sizeof index / sizeof index[0]);

  // 頂点配列オブジェクトの作成
  const GLuint vao(createObject(vertices, position, lines, index));

  // ウィンドウが開いている間繰り返す
  while (window.shouldClose() == GL_FALSE)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT);

    // シェーダプログラムの使用開始
    glUseProgram(program);

    // 投影変換行列 mp を求める (window.getAspect() はウィンドウの縦横比)
    GLfloat mp[16];
    perspective(mp, 0.5f, window.getAspect(), 1.0f, 15.0f);

    // 投影変換行列 mp とビュー変換行列 mv の積を変換行列 mc に求める
    GLfloat mc[16];
    multiply(mc, mp, mv);

    // uniform 変数 mc に変換行列 mc を設定する
    // 【宿題】ここを解答してください（uniform 変数 mc のインデックスは変数 mcLoc に入っています）

    // 描画に使う頂点配列オブジェクトの指定
    glBindVertexArray(vao);

    // 図形の描画
    glDrawElements(GL_LINES, lines, GL_UNSIGNED_INT, 0);

    // 頂点配列オブジェクトの指定解除
    glBindVertexArray(0);

    // シェーダプログラムの使用終了
    glUseProgram(0);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
