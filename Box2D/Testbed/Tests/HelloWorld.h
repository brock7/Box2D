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

#ifndef HELLOWORLD_H
#define HELLOWORLD_H

/// This stress tests the dynamic tree broad-phase. This also shows that tile
/// based collision is _not_ smooth due to Box2D not knowing about adjacency.
class HelloWorld : public Test
{
public:
	const float GroundHeight = 40.0f;
	const float GroundWidth = 40.0f;
	const int M = 5;
	const int N = 3;

	HelloWorld()
	{
		// 包围盒
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

		// 球
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-0.0f, 5.0f);
			bd.allowSleep = true;

			b2Body* body = m_world->CreateBody(&bd);
			body->SetUserData((void*)2);


			b2CircleShape shape;
			shape.m_radius = 0.7f;

			b2Fixture* fixture1 = body->CreateFixture(&shape, 0.0f);
			fixture1->SetRestitution(0.8f);
			//fixture1->SetFriction(1.0f);

			m_ball = body;

			/* shape.m_radius = 0.9f;
			b2Fixture* fixture2 = body->CreateFixture(&shape, 1.0f);
			fixture2->SetRestitution(1.0f); */
		}

		{
			b2BodyDef bd;
			bd.position.Set(-0.0f, 0.25f);
			bd.type = b2_dynamicBody;
			b2PolygonShape shape;
			shape.SetAsBox(2.0f, 2.5f, bd.position, 0.0f);
			b2Body* body = m_world->CreateBody(&bd);
			body->SetUserData((void *)3);
			m_box = body;
			b2Fixture* fixture1 = body->CreateFixture(&shape, 0.0f);
		}
		
		// 距离关节
		// 两个物体锚点的相对距离永远不变
		/* b2DistanceJointDef jd;
		jd.Initialize(m_box, m_ball, m_box->GetWorldCenter(), m_ball->GetWorldCenter());
		m_world->CreateJoint(&jd); */

		// 旋转关节
		// 
		b2RevoluteJointDef jd;
		jd.Initialize(m_box, m_ball, b2Vec2(10, 10));
		m_world->CreateJoint(&jd);

		// 移动关节
		// 两个物体的相对角度永远不变
		/* b2PrismaticJointDef jd;
		jd.Initialize(m_box, m_ball, m_box->GetWorldCenter(), b2Vec2(10, 10));
		m_world->CreateJoint(&jd); */
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
		printf("ball mass: %f\n", m_ball->GetMass());
	}

	void Keyboard(int key)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			// m_ball->ApplyForce(b2Vec2(0, 50), m_ball->GetWorldCenter(), true);
			m_ball->ApplyLinearImpulseToCenter(b2Vec2(0, 10), true);
			break;

		case GLFW_KEY_S:
			m_ball->ApplyLinearImpulseToCenter(b2Vec2(0, -10), true);
			// m_ball->ApplyForce(b2Vec2(0, -50), m_ball->GetWorldCenter(), true);
			break;
		}
		
		// m_bat->ApplyForce(, m_bat->GetWorldCenter(), true);
	}

	void KeyboardUp(int key)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			break;

		case GLFW_KEY_D:
			break;
		}
	}

	static Test* Create()
	{
		return new HelloWorld;
	}

	b2Body*			m_ball;
	b2Body*			m_box;

};

#endif
