#version 130
//The gragment shader operates on each pixel in a given polygon
//This is the 3 component float vector that gets outputted to the screen for each pixel.

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

//a uniform variable is like a global variable for the mesh (stays constant)
//any texture (2d,3d) are called samplers in GLSL
uniform sampler2D mySampler;

//cos as in cosine
void main() {

	//To get the texture from the sampler, we make a call to the texture function
	//Pass in our sampler, then we have to pass in our coordinates UV (from 0 to 1)
	//We are getting a rgba vector back from the texture function.
	
	vec4 textureColor = texture(mySampler, fragmentUV);

	//This is just for fun I think, not how it is supposed to look.
	color = textureColor * fragmentColor;
	
	color = fragmentColor * textureColor; 
}