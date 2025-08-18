#ifndef GENERADOR_H
#define GENERADOR_H

#include <string>
#include <vector>
#include "persona.h"

// ============================================================================
// FUNCIONES GENERADORAS
// ============================================================================

/**
 * Genera una fecha de nacimiento aleatoria en formato string
 * @return Fecha en formato "DD/MM/AAAA"
 */
std::string generarFechaNacimiento();

/**
 * Genera un identificador único para una persona
 * @return String con el ID generado
 */
std::string generarID();

/**
 * Genera un número decimal aleatorio dentro de un rango específico
 * @param min Valor mínimo del rango (inclusivo)
 * @param max Valor máximo del rango (inclusivo)
 * @return Número decimal aleatorio entre min y max
 */
double randomDouble(double min, double max);

/**
 * Genera una persona completa con datos aleatorios
 * @return Objeto Persona con todos los campos inicializados
 */
Persona generarPersona();

/**
 * Genera una colección de personas con datos aleatorios
 * @param n Número de personas a generar
 * @return Vector con n personas generadas aleatoriamente
 */
std::vector<Persona> generarColeccion(int n);

// ============================================================================
// FUNCIONES DE BÚSQUEDA BÁSICA
// ============================================================================

/**
 * Busca una persona por su ID en la colección
 * @param personas Vector de personas donde buscar
 * @param id ID de la persona a buscar
 * @return Puntero a la persona encontrada, nullptr si no existe
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id);

// ============================================================================
// FUNCIONES DE BÚSQUEDA POR LONGEVIDAD (EDAD)
// ============================================================================

/**
 * Busca la persona más longeva (mayor edad) - versión por valor
 * @param personas Vector de personas (se pasa por copia)
 * @return Copia de la persona más longeva
 */
Persona buscarMasLongevoPorValor(std::vector<Persona> personas);

/**
 * Busca la persona más longeva (mayor edad) - versión por referencia
 * @param personas Vector de personas (se pasa por referencia constante)
 * @return Puntero constante a la persona más longeva
 */
const Persona* buscarMasLongevoPorReferencia(const std::vector<Persona>& personas);

/**
 * Busca la persona más longeva en una ciudad específica - versión por valor
 * @param personas Vector de personas (se pasa por copia)
 * @param ciudad Nombre de la ciudad donde buscar
 * @return Copia de la persona más longeva de esa ciudad
 */
Persona buscarMasLongevoPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad);

/**
 * Busca la persona más longeva en una ciudad específica - versión por referencia
 * @param personas Vector de personas (se pasa por referencia constante)
 * @param ciudad Nombre de la ciudad donde buscar
 * @return Puntero constante a la persona más longeva de esa ciudad
 */
const Persona* buscarMasLongevoPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad);

// ============================================================================
// FUNCIONES DE BÚSQUEDA POR PATRIMONIO
// ============================================================================

/**
 * Busca la persona con mayor patrimonio - versión por valor
 * @param personas Vector de personas (se pasa por copia)
 * @return Copia de la persona con mayor patrimonio
 */
Persona buscarMasPatrimonioPorValor(std::vector<Persona> personas);

/**
 * Busca la persona con mayor patrimonio - versión por referencia
 * @param personas Vector de personas (se pasa por referencia constante)
 * @return Puntero constante a la persona con mayor patrimonio
 */
const Persona* buscarMasPatrimonioPorReferencia(const std::vector<Persona>& personas);

/**
 * Busca la persona con mayor patrimonio en una ciudad específica - versión por valor
 * @param personas Vector de personas (se pasa por copia)
 * @param ciudad Nombre de la ciudad donde buscar
 * @return Copia de la persona con mayor patrimonio de esa ciudad
 */
Persona buscarMasPatrimonioPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad);

/**
 * Busca la persona con mayor patrimonio en una ciudad específica - versión por referencia
 * @param personas Vector de personas (se pasa por referencia constante)
 * @param ciudad Nombre de la ciudad donde buscar
 * @return Puntero constante a la persona con mayor patrimonio de esa ciudad
 */
const Persona* buscarMasPatrimonioPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad);

/**
 * Busca la persona con mayor patrimonio en un grupo específico - versión por valor
 * @param personas Vector de personas (se pasa por copia)
 * @param grupo Nombre del grupo donde buscar
 * @return Copia de la persona con mayor patrimonio de ese grupo
 */
Persona buscarMasPatrimonioPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo);

/**
 * Busca la persona con mayor patrimonio en un grupo específico - versión por referencia
 * @param personas Vector de personas (se pasa por referencia constante)
 * @param grupo Nombre del grupo donde buscar
 * @return Puntero constante a la persona con mayor patrimonio de ese grupo
 */
const Persona* buscarMasPatrimonioPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo);

// ============================================================================
// FUNCIONES DE LISTADO POR GRUPO
// ============================================================================

/**
 * Lista todas las personas que pertenecen a un grupo específico - versión por valor
 * @param personas Vector de personas (se pasa por copia)
 * @param grupo Nombre del grupo a filtrar
 * @return Vector con copias de las personas del grupo especificado
 */
std::vector<Persona> listarPersonasPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo);

/**
 * Lista todas las personas que pertenecen a un grupo específico - versión por referencia
 * @param personas Vector de personas (se pasa por referencia constante)
 * @param grupo Nombre del grupo a filtrar
 * @return Vector con punteros constantes a las personas del grupo especificado
 */
std::vector<const Persona*> listarPersonasPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo);

// ============================================================================
// FUNCIONES DE VERIFICACIÓN DE GRUPOS
// ============================================================================

/**
 * Calcula el grupo correcto al que debería pertenecer una persona según su cédula
 * @param cedula Número de cédula de la persona
 * @return String con el nombre del grupo correcto
 */
std::string calcularGrupoCorrectoPorCedula(const std::string& cedula);

/**
 * Verifica si una persona está asignada al grupo correcto - versión por valor
 * @param persona Persona a verificar (se pasa por copia)
 * @return true si está en el grupo correcto, false en caso contrario
 */
bool verificarGrupoPorValor(Persona persona);

/**
 * Verifica si una persona está asignada al grupo correcto - versión por referencia
 * @param persona Persona a verificar (se pasa por referencia constante)
 * @return true si está en el grupo correcto, false en caso contrario
 */
bool verificarGrupoPorReferencia(const Persona& persona);

/**
 * Verifica masivamente si todas las personas están en sus grupos correctos - versión por valor
 * Imprime los resultados de la verificación en consola
 * @param personas Vector de personas a verificar (se pasa por copia)
 */
void verificarGruposMasivoPorValor(std::vector<Persona> personas);

/**
 * Verifica masivamente si todas las personas están en sus grupos correctos - versión por referencia
 * Imprime los resultados de la verificación en consola
 * @param personas Vector de personas a verificar (se pasa por referencia constante)
 */
void verificarGruposMasivoPorReferencia(const std::vector<Persona>& personas);

// ============================================================================
// FUNCIONES DE ANÁLISIS ESTADÍSTICO
// ============================================================================

/**
 * Encuentra el grupo con mayor patrimonio promedio - versión por valor
 * @param personas Vector de personas para analizar (se pasa por copia)
 * @return String con el nombre del grupo con mayor patrimonio promedio
 */
std::string encontrarGrupoMayorPatrimonioPorValor(std::vector<Persona> personas);

/**
 * Encuentra el grupo con mayor patrimonio promedio - versión por referencia
 * @param personas Vector de personas para analizar (se pasa por referencia constante)
 * @return String con el nombre del grupo con mayor patrimonio promedio
 */
std::string encontrarGrupoMayorPatrimonioPorReferencia(const std::vector<Persona>& personas);

/**
 * Encuentra el grupo con mayor longevidad promedio - versión por valor
 * @param personas Vector de personas para analizar (se pasa por copia)
 * @return String con el nombre del grupo con mayor longevidad promedio
 */
std::string encontrarGrupoMayorLongevidadPorValor(std::vector<Persona> personas);

/**
 * Encuentra el grupo con mayor longevidad promedio - versión por referencia
 * @param personas Vector de personas para analizar (se pasa por referencia constante)
 * @return String con el nombre del grupo con mayor longevidad promedio
 */
std::string encontrarGrupoMayorLongevidadPorReferencia(const std::vector<Persona>& personas);

// ============================================================================
// FUNCIONES DE VALIDACIÓN
// ============================================================================

/**
 * Valida si una ciudad es válida según los criterios del sistema
 * @param ciudad Nombre de la ciudad a validar
 * @return true si la ciudad es válida, false en caso contrario
 */
bool ciudadValida(const std::string& ciudad);

#endif // GENERADOR_H
