# ToolboxGraphicsとは

3DCGのためのOpenGL/Vulkan互換レイヤになる予定のもの。当面はOpenGLで開発する。

## 構成要素

* Object(メッシュ単位)
	* MODEL行列
	* 頂点配列
	* UV座標配列
	* テクスチャ
* Object(グループ)
	* MODEL行列
	* オブジェクトリスト
* Scene
	* 投影行列
	* VIEW行列
	* オブジェクトリスト
	* フレームバッファ、画面などの描画先
