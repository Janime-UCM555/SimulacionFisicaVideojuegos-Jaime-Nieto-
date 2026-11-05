#pragma once

/// <summary>
/// Interfaz base para cualquier objeto que deba actualizarse 
/// dentro del bucle principal de la aplicación. De esta forma, 
/// se puede mantener una colección heterogénea de objetos dentro de 
/// la escena y llamar a su método update() de forma polimórfica.
/// Se define un método update() virtual puro que las clases concretas 
/// implementan con su propia lógica de simulación, y un destructor virtual 
/// que garantiza la destrucción correcta a través de punteros a la clase base.
/// </summary>
class UpdatableObj
{
#pragma region Constructores y destructor
public:
	/// <summary>
	/// Destructor virtual. Permite destruir correctamente las subclases 
	/// (por ejemplo, en los vectores de punteros a UpdatableObj de las escenas).
	/// </summary>
	virtual ~UpdatableObj() = default;
#pragma endregion

#pragma region Metodos virtuales
public:
	/// <summary>
	/// Actualiza el estado del objeto.
	/// Método virtual puro: cada clase concreta define su propia lógica.
	/// </summary>
	/// <param name="dt"> Tiempo en segundos transcurrido desde la última actualización (Definido en la aplicación/main como gFixedTimestep) </param>
	virtual void update(double dt) = 0;
#pragma endregion
};