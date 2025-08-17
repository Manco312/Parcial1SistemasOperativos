#ifndef GENERADOR_H
#define GENERADOR_H

#include <string>
#include <vector>
#include "persona.h"

// Declaraciones de funciones generadoras
std::string generarFechaNacimiento();
std::string generarID();
double randomDouble(double min, double max);
Persona generarPersona();
std::vector<Persona> generarColeccion(int n);

// Declaraciones de funciones de búsqueda básica
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id);

// Declaraciones de funciones de búsqueda por longevidad
Persona buscarMasLongevoPorValor(std::vector<Persona> personas);
const Persona* buscarMasLongevoPorReferencia(const std::vector<Persona>& personas);
Persona buscarMasLongevoPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad);
const Persona* buscarMasLongevoPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad);

// Declaraciones de funciones de búsqueda por patrimonio
Persona buscarMasPatrimonioPorValor(std::vector<Persona> personas);
const Persona* buscarMasPatrimonioPorReferencia(const std::vector<Persona>& personas);
Persona buscarMasPatrimonioPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad);
const Persona* buscarMasPatrimonioPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad);
Persona buscarMasPatrimonioPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo);
const Persona* buscarMasPatrimonioPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo);

// Declaraciones de funciones de listado por grupo
std::vector<Persona> listarPersonasPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo);
std::vector<const Persona*> listarPersonasPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo);

// Declaraciones de funciones de verificación de grupos
std::string calcularGrupoCorrectoPorCedula(const std::string& cedula);
bool verificarGrupoPorValor(Persona persona);
bool verificarGrupoPorReferencia(const Persona& persona);
void verificarGruposMasivoPorValor(std::vector<Persona> personas);
void verificarGruposMasivoPorReferencia(const std::vector<Persona>& personas);

// Declaraciones de funciones de análisis estadístico
std::string encontrarGrupoMayorPatrimonioPorValor(std::vector<Persona> personas);
std::string encontrarGrupoMayorPatrimonioPorReferencia(const std::vector<Persona>& personas);
std::string encontrarGrupoMayorLongevidadPorValor(std::vector<Persona> personas);
std::string encontrarGrupoMayorLongevidadPorReferencia(const std::vector<Persona>& personas);

// Declaraciones de funciones de validación
bool ciudadValida(const std::string& ciudad);

#endif // GENERADOR_H
