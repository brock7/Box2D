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
	Pingpang()
	{
		b2BodyDef bd;
		b2Body* ground = m_world->CreateBody(&bd);

		b2Vec2 v1(-10.0f, 20.0f), v2(10.0f, 20.0f), v3(10.0f, 0.0f), v4(-10.0f, -0.0f);

		b2EdgeShape shape;

		shape.Set(v1, v2);
		ground->CreateFixture(&shape, 0.0f);

		shape.Set(v2, v3);
		ground->CreateFixture(&shape, 0.0f);

		shape.Set(v3, v4);
		ground->CreateFixture(&shape, 0.0f);

		shape.Set(v4, v1);
		ground->CreateFixture(&shape, 0.0f);


		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(-0.0f, 5.0f);
			bd.allowSleep = false;

			b2Body* body = m_world->CreateBody(&bd);

			b2CircleShape shape;
			shape.m_radius = 0.5f;

			b2Fixture* fixture1 = body->CreateFixture(&shape, 1.0f);
			fixture1->SetRestitution(1.0f);

			/* shape.m_radius = 0.9f;
			b2Fixture* fixture2 = body->CreateFixture(&shape, 1.0f);
			fixture2->SetRestitution(1.0f); */
		}
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
	}

	static Test* Create()
	{
		return new Pingpang;
	}

};

#endif
