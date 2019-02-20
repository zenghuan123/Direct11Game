#pragma once
#include "stdafx.h"
#include "gameObject.h"
#include "camera.h"
#include "tyre.h"

#ifndef VK_A 
#define VK_A 0x41
#endif
#ifndef VK_D
#define VK_D 0x44
#endif
#ifndef VK_S
#define VK_S 0x53
#endif 
#ifndef VK_W
#define VK_W 0x57
#endif



namespace Direct11Game {

	class Vehicle :
		public GameObject
	{
	private:
		bool firstPerson;
		Tyre* leftTyre;
		Tyre*rightTyre;
		Tyre*backTyre1;
		Tyre*backTyre2;




	public:
		Camera*mainCamera_;
		Vehicle(Camera*mainCamera,GameObject *parent = 0);
		virtual ~Vehicle();

		
		void Update(float t)override;
	
	};

}
