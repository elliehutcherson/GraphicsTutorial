#version 130
//The vertex shader operates on each vertex

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

void main() {
	//Set the x,y position on the screen
	gl_Position.xy = vertexPosition;
	//the z position is zero since we are in 2d
	gl_Position.z = 0.0;
	
	//indicate that the coordinates are normalized.
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	
	//Because opengl uses weird inverted vertical coordinates, we have to flip them
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
	//fragmentUV = vertexUV;
}