#include "vehicle.h"
#include "resourceManager.h"
#include "application.h"
namespace Direct11Game {

	Vehicle::Vehicle(Camera* mainCamera,GameObject *parent):GameObject(parent)
	{

		this->shader = ResourceManager::CreateShader(L"default.fx");

		MeshRenderer* cubeMesh = ResourceManager::CreateCubeMesh();
		this->meshRender_ = cubeMesh;
		wstring name = L"texture/vehicle.png";
		this->texture = ResourceManager::CreateTexture(&name, 1);		
		
		
		Tyre*tyre = new Tyre(this);
		tyre->GetTransform()->SetPosition(0.15f, 0.f, -0.05f);
		tyre->GetTransform()->SetScale(0.15f, 0.15f, 0.15f);
		leftTyre = tyre;
		tyre = new Tyre(this);
		tyre->GetTransform()->SetPosition(0.8f, 0.f, -0.05f);
		tyre->GetTransform()->SetScale(0.15f,0.15f, 0.15f);
		backTyre1 = tyre;
		tyre = new Tyre(this);
		tyre->GetTransform()->SetPosition(0.15f, 0.f, 1.05f);
		tyre->GetTransform()->SetScale(0.15f, 0.15f, 0.15f);
		rightTyre = tyre;
		tyre = new Tyre(this);
		tyre->GetTransform()->SetPosition(0.8f, 0.f, 1.05f);
		tyre->GetTransform()->SetScale(0.15f, 0.15f, 0.15f);
		backTyre2 = tyre;
		GetTransform()->SetScale(2, 1, 1);


		this->mainCamera_ = mainCamera;

	
		

		
	
	}


	Vehicle::~Vehicle()
	{
		
	}
	void Vehicle::Update(float t)
	{
		static float speed = 0.0f;
		
		/*
		turn表示在当前状态下应该转到的方向，不是每秒钟转的弧度*/
		static float turn = 0;
		/*
		每秒转的方向
		*/
		static float rotationPerSecond = 0.1f;
		static float maxSpeed = 20.f;

		static POINT lastPoint;
		/*
		用于改变camerar的target
		*/
		/*
		表示往上看还是往下看，对第一人称无效
		*/
		static float targetY=-0.5;
		/*
		x,z,合起来表示方向并且x*x+z*z=1
		*/
		static float targetX=-1;
		static float targetZ=0;
		/*
		x=-cos angle;
		z=sin angle;
		*/
		static float angle=0;
		
		/*
		表示观察点对于车子的偏移
		*/
		static float left=0;
		static float fward=0;


		if(KEYDOWN(GetAsyncKeyState(VK_W)))
		{ 
			speed += 5*t;
			if (speed > maxSpeed) {
				speed = maxSpeed;
			}
		}else if (KEYDOWN(GetAsyncKeyState(VK_S))) {
			speed -= 5*t;
			if (speed < -maxSpeed)
			{
				speed = -maxSpeed;
			}
		}
		else {
			if (speed > 0.01f)
			{
				speed -= 1.5f*t;
			}
			else if (speed <- 0.01) {
				speed += 1.5f* t;
			}
			else {
				speed = 0;
			}
		}

		if (KEYDOWN(GetAsyncKeyState(VK_A)))
		{
			turn = -0.5f;
		}else if (KEYDOWN(GetAsyncKeyState(VK_D)))
		{
				turn = 0.5f;
		}
		else {
			//turn+=fabsf(turn)*t;
			if (turn > 0)
			{
				turn -= 1*t;
			}
			else {
				turn += 1*t;
			}
		}
		
		leftTyre->SetTurn(turn);
		leftTyre->SetSpeed(speed);
		rightTyre->SetTurn(turn);
		rightTyre->SetSpeed(speed);

		backTyre1->SetSpeed(speed);
		backTyre2->SetSpeed(speed);

		if (turn > 0.01f)
		{
			//turn -= decrease * t*speed/10.f;
			XMFLOAT3 rotation = this->GetTransform()->GetRotation();
			this->GetTransform()->SetRotation(rotation.x, rotation.y + rotationPerSecond * t*speed*2 / maxSpeed, rotation.z);
		}
		else if (turn < -0.01f)
		{
			//turn += decrease * t*speed / 10.f;
			XMFLOAT3 rotation = this->GetTransform()->GetRotation();
			this->GetTransform()->SetRotation(rotation.x, rotation.y - rotationPerSecond * t*speed*2 / maxSpeed, rotation.z);
			
		}
		else {
			turn = 0;
		}




		XMFLOAT3 position = this->GetTransform()->GetPosition();
		XMVECTOR forward=XMVector3TransformNormal(XMVectorSet(-1.0f, 0, 0, 0), this->GetTransform()->GetWorldMatrix());
		forward=XMVector3Normalize(forward);
		forward=forward*speed*t;
		this->transform->SetPosition(XMVectorAdd(XMLoadFloat3(&position), forward));

		static float time =0;
		if (KEYDOWN(GetAsyncKeyState(VK_SPACE)))
		{
			if (time> 2)
			{
				firstPerson = !firstPerson;
				time = 0;
			}
		}
		time += t;
		POINT point;
		GetCursorPos(&point);
		if (KEYDOWN(GetAsyncKeyState(VK_LBUTTON)))
		{

			float x =(float)( point.x - lastPoint.x);
			float y =(float)(point.y - lastPoint.y);
			
			targetY = targetY -y / Application::GetInstance()->GetHeight()*2;
			x = x / Application::GetInstance()->GetWidth()*2;

			angle =angle+ x * XM_PIDIV4;
			targetX = -cosf(angle);
			targetZ = sinf(angle);

		}

		lastPoint.x = point.x;
		lastPoint.y = point.y;

		if (KEYDOWN(GetAsyncKeyState(VK_LEFT)))
		{
			left += t;
		}
		if (KEYDOWN(GetAsyncKeyState(VK_RIGHT)))
		{
			left -= t;
		}
		if (KEYDOWN(GetAsyncKeyState(VK_UP)))
		{
			fward += t;
		}
		if (KEYDOWN(GetAsyncKeyState(VK_DOWN)))
		{
			fward -= t;
		}


		

		/*
		摄像机是在小车坐标系当中
		*/
		if (firstPerson)
		{
			/*
			车头朝向为（-1，0，0）

			*/
			XMVECTOR po = XMVectorSet(0.f, 0, 0.5f , 1.f);
			XMVECTOR ta = XMVectorSet(-1, 0, 0.5f, 1.f);
			
			//计算坐标系转换
			XMMATRIX xm = transform->GetWorldMatrix();
			this->mainCamera_->SetPosition(XMVector3TransformCoord(po, xm));
			this->mainCamera_->SetTarget(XMVector3TransformCoord(ta, xm));
		}
		else 
		{
			XMVECTOR po = XMVectorSet(2.5f-fward, 3.f, 0.5f - left, 1.f);
			XMVECTOR ta = XMVectorSet(2.5f+targetX - fward,3.f+targetY, 0.5f+targetZ - left, 1.f);
			
			//计算坐标系转换
			XMMATRIX xm = transform->GetWorldMatrix();
			this->mainCamera_->SetPosition(XMVector3TransformCoord(po, xm));
			this->mainCamera_->SetTarget(XMVector3TransformCoord(ta, xm));
		}
		


		std::set<GameObject*>::iterator it = children.begin();
		while (it != children.end())
		{
			(*it)->Update(t);
			it++;
		}



	}


}
