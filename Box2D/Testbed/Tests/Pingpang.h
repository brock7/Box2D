/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PINGPANG_H
#define PINGPANG_H

/// This stress tests the dynamic tree broad-phase. This also shows that tile
/// based collision is _not_ smooth due to Box2D not knowing about adjacency.
class Pingpang : public Test
{
public:
	const float GroundHeight = 40.0f;
	const float GroundWidth = 40.0f;
	const int M = 5;
	const int N = 3;

	Pingpang()
	{
		m_moveDist = 0.0f;

		// АќЮЇКа
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2Vec2 v1(-GroundWidth / 2, GroundHeight), v2(GroundWidth / 2, GroundHeight), 
				v3(GroundWidth / 2, 0.0f), v4(-GroundWidth / 2, 0.0f);

			b2EdgeShape shape;

			shape.Set(v1, v2);
			ground->CreateFixture(&shape, 0.0f);

			shape.Set(v2, v3);
			ground->CreateFixture(&shape, 0.0f);

			shape.Set(v3, v4);
			ground->CreateFixture(&shape, 0.0f);

			shape.Set(v4, v1);
			ground->CreateFixture(&shape, 0.0f);
		}

		// Чђ
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-0.0f, 5.0f);
			bd.allowSleep = false;

			b2Body* body = m_world->CreateBody(&bd);
			body->SetUserData((void*)2);
			m_ball = body;

			b2CircleShape shape;
			shape.m_radius = 0.7f;

			b2Fixture* fixture1 = body->CreateFixture(&shape, 1.0f);
			fixture1->SetRestitution(0.9f);

			/* shape.m_radius = 0.9f;
			b2Fixture* fixture2 = body->CreateFixture(&shape, 1.0f);
			fixture2->SetRestitution(1.0f); */
		}

		{
			b2BodyDef bd;
			bd.position.Set(-0.0f, 0.25f);
			// bd.type = b2_dynamicBody;
			b2PolygonShape shape;
			shape.SetAsBox(4.0f, 0.25f, bd.position, 0.0f);
			b2Body* body = m_world->CreateBody(&bd);
			body->SetUserData((void *)3);
			b2Fixture* fixture1 = body->CreateFixture(&shape, 0.0f);
			fixture1->SetRestitution(2.0f);
			m_bat = body;
		}

		{
			const float BoxWidth = GroundWidth / M;
			const float BoxHeight = 2;
			b2Vec2 position;
			b2BodyDef bd;

			for (int i = 0; i < N; i ++) {
				position.y = GroundHeight - i * BoxHeight - BoxHeight / 2;
				for (int j = 0; j < M; j ++) {
					position.x = -GroundWidth / 2 + j * BoxWidth + BoxWidth / 2;
					b2PolygonShape shape;
					shape.SetAsBox(BoxWidth / 2, BoxHeight / 2, position, 0.0f);
					b2Body* body = m_world->CreateBody(&bd);
					body->SetUserData((void *)1);
					body->CreateFixture(&shape, 0.0f);
				}
			}
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);

		for (int32 i = 0; i < m_pointCount; ++i)
		{
			ContactPoint* point = m_points + i;

			b2Body* body1 = point->fixtureA->GetBody();
			b2Body* body2 = point->fixtureB->GetBody();

			if (body1->GetUserData() == (void*)2 && body2->GetUserData() == (void*)1) {

				m_world->DestroyBody(body2);
				continue;
			} else {

				if (body2->GetUserData() == (void*)2 && body1->GetUserData() == (void*)1) {
					m_world->DestroyBody(body1);
					continue;
				}
			}

			if (m_moveDist != 0) {
				if (body1->GetUserData() == (void*)2 && body2->GetUserData() == (void*)3) {
			
					body1->ApplyForce(b2Vec2(m_moveDist * 2000.0f, 0), body1->GetWorldCenter(), false);
				}
				else if (body2->GetUserData() == (void*)2 && body1->GetUserData() == (void*)3) {
		
					body2->ApplyForce(b2Vec2(m_moveDist * 2000.0f, 0), body2->GetWorldCenter(), false);
				}
			}
		}

		if (m_moveDist != 0) {

			const float BoxWidth = GroundWidth / M;

			b2Transform tr = m_bat->GetTransform();
			tr.p.x += m_moveDist;
			if (tr.p.x > (-GroundWidth / 2.0f) && tr.p.x < (GroundWidth / 2.0f)) {
				m_bat->SetTransform(tr.p, tr.q.GetAngle());
			}
		}

		int boxcount = 0;
		for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
			if (b->GetUserData() == (void*)1)
				boxcount++;

		if (boxcount == 0)
			g_debugDraw.DrawString(5, 0, "Game Over");
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_moveDist = -0.3f;
			break;

		case GLFW_KEY_D:
			m_moveDist = 0.3f;
			break;
		}
		
		// m_bat->ApplyForce(, m_bat->GetWorldCenter(), true);
	}

	void KeyboardUp(int key)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			m_moveDist = 0.0f;
			break;

		case GLFW_KEY_D:
			m_moveDist = 0.0f;
			break;
		}
	}

	static Test* Create()
	{
		return new Pingpang;
	}


	b2Body*			m_bat;
	b2Body*			m_ball;
	float			m_moveDist;
};

#endif
