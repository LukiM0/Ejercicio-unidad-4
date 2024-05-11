#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
}

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
}


void Game::DrawGame()
{
	// Función para dibujar los elementos del juego (a implementar)
	sf::RectangleShape controlador(Vector2f(3, 10));
	controlador.setFillColor(sf::Color::Magenta);
	controlador.setPosition(controlCannon->GetPosition().x + 5 , controlCannon->GetPosition().y - 4.9);
	controlador.setRotation(58);
	wnd->draw(controlador);
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt))
	{
		switch (evt.type)
		{
		case Event::Closed:
			wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
			break;
			case Event::MouseButtonPressed: //TRIANGULOS CON CLICK
			//    // Crear un cuerpo dinámico triangular en la posición del ratón
			//    b2Body* body = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
			//    // Transformar las coordenadas según la vista activa
			    Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y));
			//    body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);

			controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 9, 1.0f, 0.1f, 0.1f); //PECHO
			controlBody->SetTransform(b2Vec2(15.0f, 85.0f), 0.0f);

			b2Body* rightLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 9, 1.0f, 0.5f, 0.1f);
			rightLeg->SetTransform(b2Vec2(16.5f, 94.0f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(1.2f, 4.0f), //PIERNA DERECHA
				rightLeg, rightLeg->GetWorldCenter() - b2Vec2(0.0f, 7.0f), 0.0f, 0.1f, 1.0f);

			b2Body* leftLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 9, 1.0f, 0.5f, 0.1f);
			leftLeg->SetTransform(b2Vec2(13.5f, 94.0f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(-1.2f, 4.0f), //PIERNA IZQUIERDA
				leftLeg, leftLeg->GetWorldCenter() - b2Vec2(0.0f, 7.0f), 0.0f, 0.1f, 1.0f);


			b2Body* leftArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1, 9, 1.0f, 0.5f, 0.1f);
			leftArm->SetTransform(b2Vec2(13.5f, 85.0f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(-1.2f, -4.0f), //BRAZO IZQUIERDO
				leftArm, leftArm->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 0.1f, 1.0f);


			b2Body* rightArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 1, 9, 1.0f, 0.5f, 0.1f);
			rightArm->SetTransform(b2Vec2(16.5f, 85.0f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(1.2f, -4.0f), //BRAZO DERECHO
				rightArm, rightArm->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 0.1f, 1.0f);

			b2Body* head = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 3, 1.0f, 0.5f, 0.1f);
			head->SetTransform(b2Vec2(16.0f, 79.5f), 0.0f);

			Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(0.0f, -4.5f), //CABEZA
				head, head->GetWorldCenter(), 0.0f, 0.1f, 1.0f);

			controlBody->SetAwake(true);
			//controlBody->ApplyForceToCenter(b2Vec2(pos.x * 3000000.0f, pos.x * 3000000.0f), true);
			controlBody->SetLinearVelocity(b2Vec2((pos.x - 10)* 300.0f,(pos.y - 90)* 300.0f));
			    break;
		}
	}

	// Controlar el movimiento del cuerpo de control con el teclado
	// Segun la numeracion usada, cuando mas cerca de cero mas 
	// lento es el desplazamiento sobre ese eje


	
	
	/*
	if (Keyboard::isKeyPressed(Keyboard::Left))
		controlBody->SetLinearVelocity(b2Vec2(-30.0f, 0.0f));
	if (Keyboard::isKeyPressed(Keyboard::Right))
		controlBody->SetLinearVelocity(b2Vec2(30.0f, 0.0f));
	if (Keyboard::isKeyPressed(Keyboard::Down))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, 30.0f));
	if (Keyboard::isKeyPressed(Keyboard::Up))
		controlBody->SetLinearVelocity(b2Vec2(0.0f, -30.0f));*/

}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo y las paredes
	// También crea cuerpos dinámicos (como círculos) y los une con resortes al techo para demostrar la simulación física
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	/*b2AABB box;  
	int minx;
	int miny;
	int maxx;
	int maxy;

	box.lowerBound = b2Vec2(minx, miny); 
	box.upperBound = b2Vec2(maxx, maxy);*/

	b2Body* box1 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
	box1->SetTransform(b2Vec2(50.0f, 45.0f), 0.0f);

	b2Body* box2 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
	box2->SetTransform(b2Vec2(65.0f, 36.0f), 0.0f);

	b2Body* box3 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
	box3->SetTransform(b2Vec2(70.0f, 80.0f), 0.0f);

	b2Body* box4 = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 3);
	box4->SetTransform(b2Vec2(30.0f, 25.0f), 0.0f);


	b2Body* barrel1 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
	barrel1->SetTransform(b2Vec2(50.0f, 42.5f), 0.0f);

	b2Body* barrel2 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
	barrel2->SetTransform(b2Vec2(65.0f, 33.5f), 0.0f);

	b2Body* barrel3 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
	barrel3->SetTransform(b2Vec2(70.0f, 77.5f), 0.0f);

	b2Body* barrel4 = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 3, 5, 1.0f, 0.5f, 0.1f);
	barrel4->SetTransform(b2Vec2(30.0f, 22.5f), 0.0f);


	controlCannon = Box2DHelper::CreateRectangularStaticBody(phyWorld, 3, 10);
	controlCannon->SetTransform(b2Vec2(7.0f, 90.0f), 45.0f);


	/*
	controlBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 5, 15, 1.0f, 0.5f, 0.1f);
	controlBody->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);

	b2Body* rightLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 15, 1.0f, 0.5f, 0.1f);
	rightLeg->SetTransform(b2Vec2(52.5f, 65.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(2.0f, 7.0f), //PIERNA DERECHA
		rightLeg, rightLeg->GetWorldCenter() - b2Vec2(0.0f, 7.0f), 0.0f, 0.1f, 1.0f);

	b2Body* leftLeg = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 15, 1.0f, 0.5f, 0.1f);
	leftLeg->SetTransform(b2Vec2(47.5f, 65.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(-2.0f, 7.0f), //PIERNA IZQUIERDA
		leftLeg, leftLeg->GetWorldCenter() - b2Vec2(0.0f, 7.0f), 0.0f, 0.1f, 1.0f);


	b2Body* leftArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 10, 1.0f, 0.5f, 0.1f);
	leftArm->SetTransform(b2Vec2(47.5f, 50.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(-2.5f, -7.0f), //BRAZO IZQUIERDO
		leftArm, leftArm->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 0.1f, 1.0f);


	b2Body* rightArm = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 2, 10, 1.0f, 0.5f, 0.1f);
	rightArm->SetTransform(b2Vec2(52.5f, 50.0f), 0.0f);

	Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(2.5f, -7.0f), //BRAZO DERECHO
		rightArm, rightArm->GetWorldCenter() - b2Vec2(0.0f, 5.0f), 0.0f, 0.1f, 1.0f);

	b2Body* head = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 4, 4, 1.0f, 0.5f, 0.1f);
	head->SetTransform(b2Vec2(50.0f, 40.5f), 0.0f);
	
	Box2DHelper::CreateDistanceJoint(phyWorld, controlBody, controlBody->GetWorldCenter() + b2Vec2(0.0f, -7.5f), //CABEZA
		head, head->GetWorldCenter(), 0.0f, 0.1f, 1.0f);

	*/
}

Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}