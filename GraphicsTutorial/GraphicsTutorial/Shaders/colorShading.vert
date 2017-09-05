#version 130
//The vertex shader operates on each vertex

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

//our orthographic matrix
uniform mat4 P;

void main() {
	//Set the x,y position on the screen\
	
	//Camera2D update
	//we have to multiply our orthographic matrix (P) with
	//the vertexPosition to get our normalized device coordinates?
	//because vertexPosition is a vec2, we convert it to a vec4, set z = 0.0, and w always equals 1.0 for the moment.
	//I'm assuming w is pitch/yaw. Then we convert it back to vec2 with .xy
	
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	
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