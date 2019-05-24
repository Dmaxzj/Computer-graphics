#version 330 core
layout (location = 0) in float t;
uniform vec3 start;
uniform vec3 end;

void main()
{
	gl_Position = vec4((1-t) * start + t * end, 1.0);
}