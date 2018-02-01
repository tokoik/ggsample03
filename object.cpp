#include "gg.h"
using namespace gg;

#include "object.h"

//
// 頂点配列オブジェクトの作成
//
//   vertices: 頂点数
//   position: 頂点の二次元位置 (GLfloat[3] の配列)
//   lines: 線分数
//   index: 線分の頂点インデックス
//   戻り値: 作成された頂点配列オブジェクト名
//
GLuint createObject(GLuint vertices, const GLfloat (*position)[3], GLuint lines, const GLuint *index)
{
  // 頂点配列オブジェクト
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 頂点バッファオブジェクト
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat[3]) * vertices, position, GL_STATIC_DRAW);

  // インデックスバッファオブジェクト
  GLuint ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint) * lines, index, GL_STATIC_DRAW);

  // 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // 頂点配列オブジェクトの結合を解除した後に頂点バッファオブジェクトとインデックスバッファオブジェクトの結合を解除する
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  

  return vao;
}
