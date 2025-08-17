#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>

/**
 * Estructura que representa una persona con datos personales, fiscales y económicos.
 * 
 * POR QUÉ: Modelar los datos relevantes de una persona en un solo contenedor.
 * CÓMO: Usando un struct con atributos públicos y un constructor.
 * PARA QUÉ: Simplificar la creación, gestión y visualización de datos de personas.
 */
struct Persona {
    // --- Datos básicos ---
    std::string nombre;            // Nombre de pila
    std::string apellido;          // Apellidos
    std::string id;                // Identificador único
    std::string ciudadNacimiento;  // Ciudad de nacimiento
    std::string fechaNacimiento;   // Fecha en formato DD/MM/AAAA

    // --- Datos fiscales y demográficos ---
    std::string grupoDeclaracion;  // Grupo de declaración fiscal
    int edad;                      // Edad de la persona
    double ingresosAnuales;        // Ingresos anuales en pesos colombianos
    double patrimonio;             // Valor total de bienes y activos
    double deudas;                 // Deudas pendientes
    bool declaranteRenta;          // Si está obligado a declarar renta

    // --- Métodos de visualización ---
    void mostrar() const;        // Muestra todos los detalles completos
    void mostrarResumen() const; // Muestra versión compacta para listados
};

// ------------------- Implementaciones inline -------------------

/**
 * Implementación de mostrar.
 * 
 * POR QUÉ: Mostrar todos los datos de la persona de forma estructurada.
 * CÓMO: Usando flujos de salida y formateadores.
 * PARA QUÉ: Facilitar la lectura de los datos completos de una persona.
 */
inline void Persona::mostrar() const {
    std::cout << "\n-------------------------------------\n";
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n";
    std::cout << "   - Grupo de declaración: " << grupoDeclaracion << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "   - Edad: " << edad << " años\n";
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
}

/**
 * Implementación de mostrarResumen.
 * 
 * POR QUÉ: Mostrar información esencial de la persona en una sola línea.
 * CÓMO: Imprime ID, nombre completo, ciudad e ingresos.
 * PARA QUÉ: Listados rápidos y eficientes.
 */
inline void Persona::mostrarResumen() const {
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << ciudadNacimiento 
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales;
}

#endif // PERSONA_H
