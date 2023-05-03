#pragma once

enum Object
{
	Crate,
	Stone
};

struct ObjData
{
	vec3v v;
	vec2v uv;
	vec3v n;
};

// TODO: This class is the C++ side of a GameObject containing the draw method.
// To make this work in Lua, we will need some sort of wrapper between a C++ GameObject
// and a Lua GameObject.
class GameObject
{
public:
	ObjData data;

	void draw()
	{

	}
private:
	
};