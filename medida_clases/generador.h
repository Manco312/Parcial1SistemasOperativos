#ifndef GENERADOR_H
#define GENERADOR_H

#include "persona.h"
#include <vector>

// ============================================================================
// GENERADOR DE PERSONAS - SISTEMA DE GESTIÓN DE DATOS ALEATORIOS
// ============================================================================
// Este módulo proporciona funcionalidades para generar personas con datos
// aleatorios realistas y realizar operaciones de búsqueda y análisis sobre
// colecciones de personas.
// ============================================================================

// ============================================================================
// FUNCIONES DE VALIDACIÓN Y GENERACIÓN BÁSICA
// ============================================================================

/**
 * Valida si una ciudad es válida según criterios predefinidos.
 * 
 * @param ciudad Nombre de la ciudad a validar
 * @return true si la ciudad es válida, false en caso contrario
 * 
 * PROPÓSITO: Garantizar que solo se usen ciudades válidas en el sistema
 * IMPLEMENTACIÓN: Compara contra una lista de ciudades permitidas
 * USO: Validación de entrada antes de asignar ciudad a una persona
 */
bool ciudadValida(const std::string& ciudad);

/**
 * Genera una fecha de nacimiento aleatoria entre 1960 y 2010.
 * 
 * @return Fecha en formato string (probablemente DD/MM/YYYY)
 * 
 * PROPÓSITO: Simular fechas realistas para personas adultas
 * IMPLEMENTACIÓN: Combinando números aleatorios para día, mes y año
 * RANGO: 1960-2010 para generar personas entre 14-64 años (asumiendo año actual ~2024)
 * USO: Inicializar el atributo fechaNacimiento de Persona
 */
std::string generarFechaNacimiento();

/**
 * Genera un ID único secuencial para cada persona.
 * 
 * @return String con ID único (formato probablemente numérico secuencial)
 * 
 * PROPÓSITO: Garantizar identificadores únicos para cada persona
 * IMPLEMENTACIÓN: Contador estático que incrementa en cada llamada
 * VENTAJA: Garantiza unicidad sin colisiones
 * DESVENTAJA: No es thread-safe sin sincronización
 * USO: Asignar identificador único a cada persona generada
 */
std::string generarID();

/**
 * Genera un número decimal aleatorio en un rango específico.
 * 
 * @param min Valor mínimo del rango (inclusivo)
 * @param max Valor máximo del rango (inclusivo)
 * @return Número decimal aleatorio entre min y max
 * 
 * PROPÓSITO: Generar valores financieros realistas (ingresos, patrimonio)
 * IMPLEMENTACIÓN: Generador Mersenne Twister + distribución uniforme
 * CALIDAD: Alta calidad de aleatoriedad para simulaciones
 * USO: Generar ingresos, patrimonio, y otros valores numéricos
 */
double randomDouble(double min, double max);

// ============================================================================
// FUNCIONES DE GENERACIÓN DE PERSONAS
// ============================================================================

/**
 * Crea una persona completa con todos los datos aleatorios.
 * 
 * @return Objeto Persona con datos generados aleatoriamente
 * 
 * PROPÓSITO: Automatizar la creación completa de registros de personas
 * DATOS GENERADOS: ID, nombre, apellidos, fecha nacimiento, ciudad, ingresos, patrimonio, etc.
 * IMPLEMENTACIÓN: Combina todas las funciones generadoras + bases de datos de nombres
 * REALISMO: Usa datos coherentes y realistas para cada campo
 * USO: Crear personas individuales para pruebas o poblado de datos
 */
Persona generarPersona();

/**
 * Genera una colección de n personas con datos aleatorios.
 * 
 * @param n Número de personas a generar (debe ser > 0)
 * @return Vector conteniendo n personas generadas
 * 
 * PROPÓSITO: Crear conjuntos de datos de diferentes tamaños para pruebas
 * IMPLEMENTACIÓN: Llama a generarPersona() n veces
 * EFICIENCIA: O(n) en tiempo, cada persona se genera independientemente
 * USO: Pruebas de rendimiento, análisis estadísticos, poblado masivo de datos
 */
std::vector<Persona> generarColeccion(int n);

// ============================================================================
// FUNCIONES DE BÚSQUEDA BÁSICA
// ============================================================================

/**
 * Busca una persona específica por su ID único.
 * 
 * @param personas Vector de personas donde realizar la búsqueda
 * @param id ID de la persona a buscar
 * @return Puntero a la persona encontrada o nullptr si no existe
 * 
 * PROPÓSITO: Recuperar una persona específica de una colección
 * IMPLEMENTACIÓN: Búsqueda lineal O(n) - podría optimizarse con ordenamiento
 * RETORNO: Puntero const para evitar modificaciones accidentales
 * ERROR: Retorna nullptr si no se encuentra el ID
 * USO: Funcionalidad de búsqueda en interfaces de usuario
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id);

// ============================================================================
// FUNCIONES DE BÚSQUEDA POR LONGEVIDAD (EDAD)
// ============================================================================

/**
 * Encuentra la persona más longeva (mayor edad) - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @return Copia de la persona más longeva
 * 
 * PROPÓSITO: Identificar la persona de mayor edad en la colección
 * IMPLEMENTACIÓN: Recorre todo el vector comparando edades
 * COMPLEJIDAD: O(n) tiempo, O(n) espacio (por la copia)
 * NOTA: Menos eficiente que la versión por referencia
 */
Persona buscarMasLongevoPorValor(std::vector<Persona> personas);

/**
 * Encuentra la persona más longeva (mayor edad) - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @return Puntero const a la persona más longeva
 * 
 * PROPÓSITO: Identificar la persona de mayor edad sin copiar datos
 * IMPLEMENTACIÓN: Recorre el vector comparando edades
 * COMPLEJIDAD: O(n) tiempo, O(1) espacio
 * VENTAJA: Más eficiente en memoria que la versión por valor
 */
const Persona* buscarMasLongevoPorReferencia(const std::vector<Persona>& personas);

/**
 * Encuentra la persona más longeva en una ciudad específica - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @param ciudad Ciudad donde buscar
 * @return Copia de la persona más longeva en esa ciudad
 * 
 * PROPÓSITO: Análisis demográfico por ubicación geográfica
 * IMPLEMENTACIÓN: Filtra por ciudad y luego busca el máximo
 * COMPLEJIDAD: O(n) tiempo, O(n) espacio
 * USO: Estadísticas regionales, análisis por ciudad
 */
Persona buscarMasLongevoPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad);

/**
 * Encuentra la persona más longeva en una ciudad específica - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @param ciudad Ciudad donde buscar
 * @return Puntero const a la persona más longeva en esa ciudad
 * 
 * PROPÓSITO: Análisis demográfico eficiente por ubicación
 * IMPLEMENTACIÓN: Filtra por ciudad y busca máximo sin copiar
 * COMPLEJIDAD: O(n) tiempo, O(1) espacio
 * VENTAJA: Más eficiente para análisis de grandes volúmenes
 */
const Persona* buscarMasLongevoPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad);

// ============================================================================
// FUNCIONES DE BÚSQUEDA POR PATRIMONIO
// ============================================================================

/**
 * Encuentra la persona con mayor patrimonio - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @return Copia de la persona con mayor patrimonio
 * 
 * PROPÓSITO: Identificar la persona más adinerada
 * IMPLEMENTACIÓN: Recorre comparando valores de patrimonio
 * USO: Análisis financiero, identificación de personas de alto patrimonio
 */
Persona buscarMasPatrimonioPorValor(std::vector<Persona> personas);

/**
 * Encuentra la persona con mayor patrimonio - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @return Puntero const a la persona con mayor patrimonio
 * 
 * PROPÓSITO: Identificar eficientemente la persona más adinerada
 * VENTAJA: Sin overhead de copia para grandes estructuras de datos
 */
const Persona* buscarMasPatrimonioPorReferencia(const std::vector<Persona>& personas);

/**
 * Encuentra la persona con mayor patrimonio en una ciudad - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @param ciudad Ciudad donde buscar
 * @return Copia de la persona con mayor patrimonio en esa ciudad
 * 
 * PROPÓSITO: Análisis financiero regional
 * USO: Identificar personas adineradas por ubicación geográfica
 */
Persona buscarMasPatrimonioPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad);

/**
 * Encuentra la persona con mayor patrimonio en una ciudad - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @param ciudad Ciudad donde buscar
 * @return Puntero const a la persona con mayor patrimonio en esa ciudad
 * 
 * PROPÓSITO: Análisis financiero regional eficiente
 */
const Persona* buscarMasPatrimonioPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad);

/**
 * Encuentra la persona con mayor patrimonio en un grupo específico - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @param grupo Identificador del grupo (probablemente basado en cédula)
 * @return Copia de la persona con mayor patrimonio en ese grupo
 * 
 * PROPÓSITO: Análisis financiero por segmentación demográfica
 * USO: Comparaciones entre diferentes grupos poblacionales
 */
Persona buscarMasPatrimonioPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo);

/**
 * Encuentra la persona con mayor patrimonio en un grupo específico - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @param grupo Identificador del grupo
 * @return Puntero const a la persona con mayor patrimonio en ese grupo
 * 
 * PROPÓSITO: Análisis financiero eficiente por segmentación
 */
const Persona* buscarMasPatrimonioPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo);

// ============================================================================
// FUNCIONES DE LISTADO Y FILTRADO
// ============================================================================

/**
 * Lista todas las personas pertenecientes a un grupo específico - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @param grupo Identificador del grupo a filtrar
 * @return Vector con copias de todas las personas del grupo
 * 
 * PROPÓSITO: Obtener subconjuntos de personas por criterio de grupo
 * IMPLEMENTACIÓN: Filtra y copia personas que coincidan con el grupo
 * USO: Generar reportes por segmentos, análisis grupales
 */
std::vector<Persona> listarPersonasPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo);

/**
 * Lista todas las personas pertenecientes a un grupo específico - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @param grupo Identificador del grupo a filtrar
 * @return Vector de punteros const a las personas del grupo
 * 
 * PROPÓSITO: Obtener subconjuntos eficientemente sin copiar datos
 * VENTAJA: Menor uso de memoria para grandes colecciones
 */
std::vector<const Persona*> listarPersonasPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo);

// ============================================================================
// FUNCIONES DE VALIDACIÓN Y VERIFICACIÓN DE GRUPOS
// ============================================================================

/**
 * Calcula el grupo correcto basado en el número de cédula.
 * 
 * @param cedula Número de cédula de la persona
 * @return String identificando el grupo correcto
 * 
 * PROPÓSITO: Determinar la clasificación grupal según algoritmo de cédula
 * IMPLEMENTACIÓN: Probablemente usa dígitos específicos o módulo de la cédula
 * USO: Validar asignaciones de grupo, corregir datos inconsistentes
 */
std::string calcularGrupoCorrectoPorCedula(const std::string& cedula);

/**
 * Verifica si una persona está asignada al grupo correcto - versión por valor.
 * 
 * @param persona Persona a verificar (copia por valor)
 * @return true si el grupo asignado coincide con el calculado por cédula
 * 
 * PROPÓSITO: Control de calidad de datos, detección de inconsistencias
 * IMPLEMENTACIÓN: Compara grupo actual vs grupo calculado por cédula
 */
bool verificarGrupoPorValor(Persona persona);

/**
 * Verifica si una persona está asignada al grupo correcto - versión por referencia.
 * 
 * @param persona Persona a verificar (referencia constante)
 * @return true si el grupo asignado es correcto
 * 
 * PROPÓSITO: Verificación eficiente sin copia de datos
 */
bool verificarGrupoPorReferencia(const Persona& persona);

/**
 * Verifica masivamente la correctitud de grupos para toda una colección - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * 
 * PROPÓSITO: Auditoría completa de calidad de datos
 * IMPLEMENTACIÓN: Verifica cada persona y probablemente reporta inconsistencias
 * USO: Validación de integridad después de importar datos
 */
void verificarGruposMasivoPorValor(std::vector<Persona> personas);

/**
 * Verifica masivamente la correctitud de grupos para toda una colección - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * 
 * PROPÓSITO: Auditoría eficiente de calidad de datos
 * VENTAJA: Sin overhead de copia para verificaciones masivas
 */
void verificarGruposMasivoPorReferencia(const std::vector<Persona>& personas);

// ============================================================================
// FUNCIONES DE ANÁLISIS ESTADÍSTICO POR GRUPOS
// ============================================================================

/**
 * Encuentra el grupo con mayor patrimonio promedio/total - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @return String identificando el grupo con mayor patrimonio
 * 
 * PROPÓSITO: Análisis comparativo de riqueza entre grupos demográficos
 * IMPLEMENTACIÓN: Calcula estadísticas por grupo y encuentra el máximo
 * USO: Estudios socioeconómicos, análisis de desigualdad
 */
std::string encontrarGrupoMayorPatrimonioPorValor(std::vector<Persona> personas);

/**
 * Encuentra el grupo con mayor patrimonio promedio/total - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @return String identificando el grupo con mayor patrimonio
 * 
 * PROPÓSITO: Análisis eficiente de riqueza por grupos
 */
std::string encontrarGrupoMayorPatrimonioPorReferencia(const std::vector<Persona>& personas);

/**
 * Encuentra el grupo con mayor longevidad promedio - versión por valor.
 * 
 * @param personas Vector de personas (copia por valor)
 * @return String identificando el grupo más longevo
 * 
 * PROPÓSITO: Análisis demográfico de esperanza de vida por grupos
 * IMPLEMENTACIÓN: Calcula edad promedio por grupo y encuentra el máximo
 * USO: Estudios de salud pública, análisis actuariales
 */
std::string encontrarGrupoMayorLongevidadPorValor(std::vector<Persona> personas);

/**
 * Encuentra el grupo con mayor longevidad promedio - versión por referencia.
 * 
 * @param personas Vector de personas (referencia constante)
 * @return String identificando el grupo más longevo
 * 
 * PROPÓSITO: Análisis eficiente de longevidad por grupos
 */
std::string encontrarGrupoMayorLongevidadPorReferencia(const std::vector<Persona>& personas);

#endif // GENERADOR_H
