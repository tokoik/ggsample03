//
// 頂点配列オブジェクトの作成
//
//   vertices: 頂点数
//   position: 頂点の二次元位置 (GLfloat[3] の配列)
//   lines: 線分数
//   index: 線分の頂点インデックス
//   戻り値: 作成された頂点配列オブジェクト名
//
extern GLuint createObject(GLuint vertices, const GLfloat(*position)[3], GLuint lines, const GLuint* index);
