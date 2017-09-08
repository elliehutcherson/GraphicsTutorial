#include "Camera2D.h"

namespace GameEngine {

	Camera2D::Camera2D() :
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f),
		_scale(1.0f),
		_needsMatrixUpdate(true),
		_screenWidth(500),
		_screenHeight(500)
	{
	}

	//Setting these 4x4 matrix equal to the identity matrix
	//			| 1 0 0 0 | not exactly sure how 1.0f equals
	//			| 0 1 0 0 | the identity matrix, but I guess
	//			| 0 0 1 0 | that works.
	//			| 0 0 0 1 |

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight) {
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		//We only need to build this orthoMatrix (orthographic matrix) whenever our screenWidth or screenHeight changes.
		//we have to convert _screenWidth and _screenHeight to floats from int
		//orthographic projection is basically what 3-dimensional objects look like on a 2d dimensional surface, especially
		//if you were moving, and how that effects objects in the background from your perspective.
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	}

	void Camera2D::update() {

		if (_needsMatrixUpdate) {
			//A translation is a transformation that moves our objects around
			//So we have to move our entire view port. So when we go to the
			//right, everything else goes to the left, hence giving our position 
			//as negative.

			//We also have to make sure that this matrix is going off the center of our 
			//position by adding our position(x,y) to half of the screen dimensions (w,h)

			//Camera Translation
			glm::vec3 translate(-_position.x + _screenWidth/2, -_position.y + _screenHeight/2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);
			
			//Camera Scale
			//So we create a scale matrix based off our _scale variable which is a float.
			//Then we scale an identity matrix with our scale matrix, and multiply that by our camera matrix.
			//This is so that the scaling is based off our centered position.
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false; //Whenever it needs to be updated again, we'll set it to true.
		}
	}

	glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
		//Make it so that 0 is the center
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
		//Scale the coordinates
		screenCoords /= _scale;
		// Translate with the camera position
		screenCoords += _position;
		return screenCoords;
	}
}
