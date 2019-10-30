--[[
	This file lists every asset that must be built by the AssetBuildSystem
	F:\Users\HP\Documents\zhu_an\MyGame_\Content\Shaders\Fragment
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/test.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout_3dObject.shader", arguments = { "vertex" } },
	},
]]

return
{
shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
	},
	effect = {
		path = "Effect/basic.lua"
	},
	geometry ={
		{ path = "Geometry/Triangle.lua"},
		{ path = "Geometry/Plane.lua"},
		{ path = "Geometry/Cube.lua"},
		{ path = "Geometry/Sphere.lua"},
	},
}
