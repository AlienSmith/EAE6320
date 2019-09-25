// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

// Inherited Implementation
//=========================

// Run
//----

void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	//Update position
	m_camera.position += m_camera.velocity * i_elapsedSecondCount_sinceLastSimulationUpdate;
	Graphics::SubmitClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	eae6320::Graphics::sCameranPerspective temp;
	temp.kinematic = &m_camera;
	Graphics::SubmitCameraData(temp);
	if (m_changeeffect) {
		Graphics::SubmitEffectWithObject(m_flash_Effect, m_quard);
	}
	else {
		Graphics::SubmitEffectWithObject(m_white_Effect, m_quard);
	}
	if (m_showotherobject) {
		Graphics::SubmitEffectWithObject(m_white_Effect, m_triangle);
	}
}

void eae6320::cExampleGame::UpdateBasedOnInput()
{
	//Make the trasform
	Math::sVector temp_vol = Math::sVector(0.0f, 0.0f, 0.0f);
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up)) {
		temp_vol.z = -1.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down)) {
		temp_vol.z = 1.0f;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left)) {
		temp_vol.x = -1.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right)) {
		temp_vol.x = 1.0f;
	}
	m_camera.velocity = temp_vol;
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space)) {
		m_showotherobject = true;
	}
	else {
		m_showotherobject = false;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter)) {
		m_changeeffect = true;
	}
	else {
		m_changeeffect = false;
	}
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	using namespace Graphics;
	sDataRequriedToIntializeObject squre;
	squre.indexcount = 6;
	squre.vertexcount = 4;
	eae6320::Graphics::VertexFormats::s3dObject temp_vertexData[4];
	{
		temp_vertexData[0].x = 0.0f;
		temp_vertexData[0].y = 0.0f;
		temp_vertexData[0].z = 0.0f;

		temp_vertexData[1].x = 1.0f;
		temp_vertexData[1].y = 0.0f;
		temp_vertexData[1].z = 0.0f;

		temp_vertexData[2].x = 1.0f;
		temp_vertexData[2].y = 1.0f;
		temp_vertexData[2].z = 0.0f;

		temp_vertexData[3].x = 0.0f;
		temp_vertexData[3].y = 1.0f;
		temp_vertexData[3].z = 0.0f;
	}
	squre.vertexData = temp_vertexData;
	uint16_t temp_indexData[6]{ 0,1,2,0,2,3 };
	squre.indexdata = temp_indexData;
	DefaultGeometry::Create(squre, m_quard);

	sDataRequriedToIntializeObject triangle;
	triangle.indexcount = 3;
	triangle.vertexcount = 3;
	eae6320::Graphics::VertexFormats::s3dObject triangle_vertexData[3];
	{
		triangle_vertexData[0].x = 0.0f;
		triangle_vertexData[0].y = 0.0f;
		triangle_vertexData[0].z = 0.0f;

		triangle_vertexData[1].x = -1.0f;
		triangle_vertexData[1].y = -1.0f;
		triangle_vertexData[1].z = 0.0f;

		triangle_vertexData[2].x = 0.0f;
		triangle_vertexData[2].y = -1.0f;
		triangle_vertexData[2].z = 0.0f;
	}
	triangle.vertexData = triangle_vertexData;
	uint16_t triangle_indexData[3]{ 0,1,2 };
	triangle.indexdata = triangle_indexData;
	DefaultGeometry::Create(triangle, m_triangle);

	sDataRequriedToIntializeEffect flesh_data;
	flesh_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
	flesh_data.fragment_shader_path = "data/Shaders/Fragment/test.shader";
	cEffect::Create(flesh_data, m_flash_Effect);

	sDataRequriedToIntializeEffect white_data;
	white_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
	white_data.fragment_shader_path = "data/Shaders/Fragment/standard.shader";
	cEffect::Create(white_data, m_white_Effect);
	int a = sizeof(cEffect);
	int b = sizeof(DefaultGeometry);
	m_camera.position = Math::sVector(0.0f,0.0f,10.0f);
	return Results::Success;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	m_flash_Effect->DecrementReferenceCount();
	m_white_Effect->DecrementReferenceCount();
	m_quard->DecrementReferenceCount();
	m_triangle->DecrementReferenceCount();
	m_flash_Effect->DecrementReferenceCount();
	m_white_Effect->DecrementReferenceCount();
	m_quard->DecrementReferenceCount();
	m_triangle->DecrementReferenceCount();
	return Results::Success;
}
