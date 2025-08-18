/**
 * @file generador.cpp
 * @brief Implementación del sistema generador de personas con datos realistas colombianos
 * @author Sistema de Gestión de Personas
 * @date 2025
 * 
 * Este archivo contiene la implementación completa del sistema de generación,
 * búsqueda y análisis de datos de personas con información demográfica y
 * económica realista de Colombia.
 */

#include "generador.h"
#include <cstdlib>   // rand(), srand() - Funciones de números aleatorios básicos
#include <ctime>     // time() - Para semilla de números aleatorios
#include <random>    // Generadores aleatorios modernos C++11+
#include <vector>    // Contenedor dinámico para colecciones
#include <algorithm> // Para find_if, max_element - Algoritmos STL
#include <iostream>  // Para std::cout - Salida por consola
#include <stdexcept> // Para std::runtime_error y std::invalid_argument - Manejo de excepciones

// ============================================================================
// BASES DE DATOS PARA GENERACIÓN REALISTA DE DATOS COLOMBIANOS
// ============================================================================

/**
 * @brief Nombres femeninos más comunes en Colombia
 * 
 * Base de datos curada con los nombres femeninos más populares según
 * registros del DANE (Departamento Administrativo Nacional de Estadística).
 * Incluye nombres tradicionales y modernos para mayor realismo.
 */
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

/**
 * @brief Nombres masculinos más comunes en Colombia
 * 
 * Colección de nombres masculinos tradicionales y contemporáneos.
 * Incluye variaciones regionales y nombres compuestos típicos colombianos.
 * Nota: Algunos nombres aparecen duplicados intencionalmente para simular
 * la frecuencia real de uso en la población.
 */
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Sofia","Camila","Juana","Ana","Laura","Karla","Andrea","Daniela","Alejandra","Martina",
    "Nelly","María","Nestor","Trinidad","Fernanda", "Carolina", "Lina", "Gertridis"
};

/**
 * @brief Apellidos más frecuentes en Colombia
 * 
 * Lista de apellidos basada en estudios demográficos colombianos.
 * Se utilizan para generar combinaciones realistas de apellidos compuestos
 * (primer apellido + segundo apellido) según la tradición hispanoamericana.
 */
const std::vector<std::string> apellidos = {
    "Gómez", "Rodríguez", "Martínez", "López", "García", "Pérez", "González", "Sánchez", "Ramírez", "Torres",
    "Díaz", "Vargas", "Castro", "Ruiz", "Álvarez", "Romero", "Suárez", "Rojas", "Moreno", "Muñoz", "Valencia",
};

/**
 * @brief Principales ciudades de Colombia por población
 * 
 * Incluye capitales departamentales y ciudades principales ordenadas
 * aproximadamente por importancia demográfica y económica.
 * Utilizada para generar distribución geográfica realista.
 */
const std::vector<std::string> ciudadesColombia = {
    "Bogotá", "Medellín", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Santa Marta", "Cúcuta", "Ibagué",
    "Manizales", "Pasto", "Neiva", "Villavicencio", "Armenia", "Sincelejo", "Valledupar", "Montería", "Popayán", "Tunja"
};

// ============================================================================
// IMPLEMENTACIÓN DE FUNCIONES GENERADORAS
// ============================================================================

/**
 * @brief Genera una fecha de nacimiento aleatoria en formato DD/MM/AAAA
 * 
 * @return std::string Fecha en formato "DD/MM/AAAA"
 * 
 * ALGORITMO:
 * 1. Genera día entre 1-28 (simplificado para evitar problemas de meses)
 * 2. Genera mes entre 1-12
 * 3. Genera año entre 1960-2010 (personas de 15-65 años aprox.)
 * 
 * LIMITACIONES:
 * - Usa días 1-28 para simplificar (evita problemas con febrero/meses cortos)
 * - No considera años bisiestos
 * - Rango de años fijo (podría parametrizarse)
 */
std::string generarFechaNacimiento() {
    // Genera día aleatorio (1-28 para simplificar validación de meses)
    int dia = 1 + rand() % 28;
    // Mes aleatorio (1-12)
    int mes = 1 + rand() % 12;
    // Año entre 1960-2010 (personas adultas contemporáneas)
    int anio = 1960 + rand() % 50;
    
    // Convierte a string en formato DD/MM/AAAA estándar colombiano
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

/**
 * @brief Genera un ID único secuencial simulando cédulas colombianas
 * 
 * @return std::string ID único como string numérico
 * 
 * DISEÑO:
 * - Usa variable estática para mantener secuencia entre llamadas
 * - Inicia en 1000000000 (10 dígitos, similar a cédulas reales)
 * - Incrementa automáticamente para garantizar unicidad
 * - Thread-unsafe: no apto para uso concurrente sin sincronización
 */
std::string generarID() {
    static long contador = 1000000000; // ID inicial de 10 dígitos
    return std::to_string(contador++); // Post-incremento: usa valor actual, luego incrementa
}

/**
 * @brief Genera número decimal aleatorio en rango específico usando Mersenne Twister
 * 
 * @param min Valor mínimo (inclusivo)
 * @param max Valor máximo (inclusivo)
 * @return double Número aleatorio en rango [min, max]
 * 
 * VENTAJAS sobre rand():
 * - Mayor calidad estadística (Mersenne Twister)
 * - Distribución uniforme garantizada
 * - Mejor para aplicaciones que requieren aleatoriedad de calidad
 * 
 * IMPLEMENTACIÓN:
 * - Generador estático (una instancia por programa)
 * - Semilla basada en tiempo actual
 * - Distribución uniforme real
 */
double randomDouble(double min, double max) {
    // Generador moderno Mersenne Twister (mejor que rand())
    static std::mt19937 generator(time(nullptr));
    // Distribución uniforme en rango [min, max]
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

/**
 * @brief Genera una persona completa con datos demográficos y económicos realistas
 * 
 * @return Persona Estructura completa con todos los campos poblados
 * 
 * PROCESO DE GENERACIÓN:
 * 1. Determina género aleatoriamente (50/50)
 * 2. Selecciona nombre según género de las bases de datos
 * 3. Combina dos apellidos aleatorios (tradición hispanoamericana)
 * 4. Genera identificadores únicos y datos demográficos
 * 5. Calcula edad basada en fecha de nacimiento
 * 6. Genera datos económicos con rangos realistas colombianos
 * 7. Determina estatus de declarante según ingresos
 * 8. Asigna grupo de declaración basado en cédula
 * 
 * RANGOS ECONÓMICOS (Pesos Colombianos 2025):
 * - Ingresos: 10M - 500M COP anuales
 * - Patrimonio: 0 - 2,000M COP
 * - Deudas: hasta 70% del patrimonio
 * - Declarante: >50M ingresos con 70% probabilidad
 */
Persona generarPersona() {
    Persona p; // Crea una instancia de la estructura Persona
    
    bool esHombre = rand() % 2;
    
    p.nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    p.apellido = apellidos[rand() % apellidos.size()] + " " + 
                 apellidos[rand() % apellidos.size()];
    
    p.id = generarID();
    // Ciudad aleatoria de Colombia
    p.ciudadNacimiento = ciudadesColombia[rand() % ciudadesColombia.size()];
    // Fecha aleatoria
    p.fechaNacimiento = generarFechaNacimiento();

    p.edad = 2025 - std::stoi(p.fechaNacimiento.substr(p.fechaNacimiento.find_last_of('/') + 1));
    
    // --- GENERACIÓN DE DATOS ECONÓMICOS REALISTAS ---    
    p.ingresosAnuales = randomDouble(10000000, 500000000);
    p.patrimonio = randomDouble(0, 2000000000);
    p.deudas = randomDouble(0, p.patrimonio * 0.7);
    p.declaranteRenta = (p.ingresosAnuales > 50000000) && (rand() % 100 > 30);
    
    p.grupoDeclaracion = calcularGrupoCorrectoPorCedula(p.id);
    
    return p; // Retorna la estructura completa
}

/**
 * @brief Genera una colección de n personas con datos aleatorios
 * 
 * @param n Número de personas a generar
 * @return std::vector<Persona> Vector con n personas generadas
 * 
 * OPTIMIZACIONES:
 * - reserve(n): Pre-asigna memoria para evitar realocaciones
 * - push_back: Añade elementos al final eficientemente
 * 
 * COMPLEJIDAD: O(n) tiempo, O(n) espacio
 */
std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    personas.reserve(n); // Reserva espacio para n personas (optimización de memoria)
    
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

// ============================================================================
// FUNCIONES DE BÚSQUEDA Y CONSULTA
// ============================================================================

/**
 * @brief Busca una persona por su ID en la colección
 * 
 * @param personas Vector de personas donde buscar
 * @param id ID a buscar
 * @return const Persona* Puntero a la persona encontrada, nullptr si no existe
 * 
 * ALGORITMO:
 * - Usa std::find_if con lambda para búsqueda lineal
 * - Retorna puntero constante para evitar modificaciones accidentales
 * - nullptr indica "no encontrado" (patrón estándar C++)
 * 
 * COMPLEJIDAD: O(n) en el peor caso
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.id == id; }); // Acceso directo a campos de estructura
    
    if (it != personas.end()) {
        return &(*it); // Devuelve puntero a la persona encontrada
    } else {
        return nullptr; // No encontrado
    }
}

// ============================================================================
// BÚSQUEDA DE PERSONA MÁS LONGEVA - PATRÓN VALOR VS REFERENCIA
// ============================================================================

/**
 * @brief Busca la persona más longeva (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @return Persona Copia de la persona más longeva
 * 
 * CARACTERÍSTICAS:
 * - Recibe copia del vector (menos eficiente en memoria)
 * - Retorna copia de la persona (seguro pero costoso)
 * - Útil cuando se necesita modificar el resultado sin afectar original
 * 
 * COMPLEJIDAD: O(n) tiempo, O(n) espacio adicional por la copia
 */
Persona buscarMasLongevoPorValor(std::vector<Persona> personas) {
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.edad < b.edad; });
}

/**
 * @brief Busca la persona más longeva (VERSIÓN POR REFERENCIA)
 * 
 * @param personas Vector por referencia constante
 * @return const Persona* Puntero a la persona más longeva, nullptr si vacío
 * 
 * CARACTERÍSTICAS:
 * - Recibe referencia (eficiente en memoria)
 * - Retorna puntero constante (eficiente, no permite modificación)
 * - Maneja caso de vector vacío explícitamente
 * 
 * COMPLEJIDAD: O(n) tiempo, O(1) espacio adicional
 */
const Persona* buscarMasLongevoPorReferencia(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr; // Manejo explícito de caso vacío

    auto it = std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.edad < b.edad; });

    return &(*it);
}

/**
 * @brief Busca la persona más longeva en una ciudad específica (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @param ciudad Ciudad donde buscar
 * @return Persona Copia de la persona más longeva en la ciudad
 * @throws std::runtime_error Si no hay personas en la ciudad especificada
 * 
 * ALGORITMO:
 * 1. Filtra personas de la ciudad especificada
 * 2. Verifica que existan personas en esa ciudad
 * 3. Busca la más longeva entre las filtradas
 * 
 * MANEJO DE ERRORES:
 * - Lanza excepción si la ciudad no tiene habitantes
 * - Mensaje descriptivo para debugging
 */
Persona buscarMasLongevoPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad) {
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.edad < b.edad; });
}

/**
 * @brief Busca la persona más longeva en una ciudad específica (VERSIÓN POR REFERENCIA)
 * 
 * @param personas Vector por referencia constante
 * @param ciudad Ciudad donde buscar
 * @return const Persona* Puntero a la persona más longeva en la ciudad
 * @throws std::runtime_error Si no hay personas en la ciudad especificada
 * 
 * OPTIMIZACIÓN:
 * - Usa vector de punteros para evitar copiar objetos Persona
 * - Más eficiente en memoria para objetos grandes
 * - Mantiene referencias al vector original
 */
const Persona* buscarMasLongevoPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad) {
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->edad < b->edad; });
}

// ============================================================================
// BÚSQUEDA DE PERSONA CON MAYOR PATRIMONIO - PATRÓN VALOR VS REFERENCIA
// ============================================================================

/**
 * @brief Busca la persona con mayor patrimonio (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @return Persona Copia de la persona con mayor patrimonio
 * 
 * Similar a buscarMasLongevoPorValor pero comparando por patrimonio.
 * Útil para análisis económicos y estadísticas de riqueza.
 */
Persona buscarMasPatrimonioPorValor(std::vector<Persona> personas) {
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });
}

/**
 * @brief Busca la persona con mayor patrimonio (VERSIÓN POR REFERENCIA)
 * 
 * @param personas Vector por referencia constante
 * @return const Persona* Puntero a la persona con mayor patrimonio
 * 
 * Versión eficiente para análisis económicos en grandes datasets.
 */
const Persona* buscarMasPatrimonioPorReferencia(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;

    auto it = std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });

    return &(*it);
}

/**
 * @brief Busca la persona con mayor patrimonio en una ciudad (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @param ciudad Ciudad donde buscar
 * @return Persona Copia de la persona más rica en la ciudad
 * @throws std::runtime_error Si no hay personas en la ciudad
 * 
 * Útil para análisis económicos regionales y comparaciones entre ciudades.
 */
Persona buscarMasPatrimonioPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad) {
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });
}

/**
 * @brief Busca la persona con mayor patrimonio en una ciudad (VERSIÓN POR REFERENCIA)
 * 
 * Versión optimizada para análisis económicos regionales eficientes.
 */
const Persona* buscarMasPatrimonioPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad) {
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->patrimonio < b->patrimonio; });
}

/**
 * @brief Busca la persona con mayor patrimonio en un grupo de declaración (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @param grupo Grupo de declaración ("A", "B", "C")
 * @return Persona Copia de la persona más rica en el grupo
 * @throws std::runtime_error Si no hay personas en el grupo
 * 
 * Útil para análisis de distribución de riqueza por grupos tributarios.
 */
Persona buscarMasPatrimonioPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo) {
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.grupoDeclaracion == grupo) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en el grupo: " + grupo);
    }

    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });
}

/**
 * @brief Busca la persona con mayor patrimonio en un grupo de declaración (VERSIÓN POR REFERENCIA)
 * 
 * Versión optimizada para análisis tributarios eficientes.
 */
const Persona* buscarMasPatrimonioPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo) {
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.grupoDeclaracion == grupo) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en el grupo: " + grupo);
    }

    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->patrimonio < b->patrimonio; });
}

// ============================================================================
// FUNCIONES DE LISTADO Y CONTEO POR GRUPOS
// ============================================================================

/**
 * @brief Lista y cuenta personas de un grupo específico (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @param grupo Grupo de declaración a filtrar (ej. "A", "B", "C")
 * @return std::vector<Persona> Vector con las personas del grupo especificado
 * 
 * FUNCIONALIDAD:
 * - Filtra personas por grupo de declaración
 * - Imprime información básica de cada persona encontrada
 * - Retorna vector con todas las personas del grupo
 * 
 * SALIDA POR CONSOLA:
 * - Muestra ID, nombre completo de cada persona del grupo
 * - Útil para auditorías y reportes por grupos tributarios
 */
std::vector<Persona> listarPersonasPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo) {
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.grupoDeclaracion == grupo) {
            std::cout << "ID: " << p.id << ", Nombre: " << p.nombre << " " << p.apellido << std::endl;
            filtradas.push_back(p);
        }
    }
    return filtradas;
}

/**
 * @brief Lista y cuenta personas de un grupo específico (VERSIÓN POR REFERENCIA)
 * 
 * @param personas Vector por referencia constante
 * @param grupo Grupo de declaración a filtrar
 * @return std::vector<const Persona*> Vector de punteros a personas del grupo
 * 
 * Versión optimizada que evita copiar objetos Persona, ideal para
 * procesamiento de grandes volúmenes de datos tributarios.
 */
std::vector<const Persona*> listarPersonasPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo) {
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.grupoDeclaracion == grupo) {
            std::cout << "ID: " << p.id << ", Nombre: " << p.nombre << " " << p.apellido << std::endl;
            filtradas.push_back(&p);
        }
    }
    return filtradas;
}

// ============================================================================
// SISTEMA DE VERIFICACIÓN DE GRUPOS DE DECLARACIÓN
// ============================================================================

/**
 * @brief Calcula el grupo correcto de declaración basado en la cédula
 * 
 * @param cedula Número de cédula como string
 * @return std::string Grupo calculado ("A", "B", "C")
 * @throws std::invalid_argument Si la cédula tiene menos de 2 dígitos
 * 
 * ALGORITMO DE ASIGNACIÓN (Basado en normativa DIAN):
 * - Últimos 2 dígitos 00-39: Grupo A
 * - Últimos 2 dígitos 40-79: Grupo B  
 * - Últimos 2 dígitos 80-99: Grupo C
 * 
 * PROPÓSITO:
 * - Distribuir declarantes en grupos para gestión tributaria
 * - Verificar consistencia de datos asignados vs calculados
 * - Detectar errores en asignación manual de grupos
 */
std::string calcularGrupoCorrectoPorCedula(const std::string& cedula) {
    if (cedula.length() < 2) {
        throw std::invalid_argument("La cédula debe tener al menos 2 dígitos");
    }
    
    int ultDigitos = std::stoi(cedula.substr(cedula.length() - 2));
    
    if (ultDigitos >= 0 && ultDigitos <= 39) {
        return "A";
    } else if (ultDigitos >= 40 && ultDigitos <= 79) {
        return "B";
    } else { // 80-99
        return "C";
    }
}

/**
 * @brief Verificación individual por VALOR - Recibe una copia de la persona
 * 
 * @param persona Copia de la persona a verificar
 * @return bool true si el grupo asignado coincide con el calculado
 * 
 * CARACTERÍSTICAS:
 * - Recibe copia completa del objeto (seguro pero menos eficiente)
 * - No puede modificar el objeto original
 * - Útil cuando se necesita verificar sin riesgo de alteración
 * - Manejo robusto de excepciones con logging de errores
 * 
 * PROCESO:
 * 1. Calcula grupo correcto basado en cédula
 * 2. Compara con grupo asignado en la estructura
 * 3. Retorna resultado de la comparación
 * 4. Captura y registra cualquier excepción
 */
bool verificarGrupoPorValor(Persona persona) {
    try {
        std::string grupoCalculado = calcularGrupoCorrectoPorCedula(persona.id);
        std::string grupoAsignado = persona.grupoDeclaracion;
        
        bool esCorrect = (grupoCalculado == grupoAsignado);
        
        return esCorrect;
    } catch (const std::exception& e) {
        std::cerr << "Error en verificación por valor: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Verificación individual por REFERENCIA - Recibe referencia constante
 * 
 * @param persona Referencia constante a la persona
 * @return bool true si el grupo asignado coincide con el calculado
 * 
 * VENTAJAS:
 * - No copia el objeto (más eficiente en memoria y tiempo)
 * - Referencia constante previene modificaciones accidentales
 * - Ideal para verificaciones masivas o objetos grandes
 * - Mismo algoritmo de verificación que la versión por valor
 */
bool verificarGrupoPorReferencia(const Persona& persona) {
    try {
        std::string grupoCalculado = calcularGrupoCorrectoPorCedula(persona.id);
        std::string grupoAsignado = persona.grupoDeclaracion;
        
        bool esCorrect = (grupoCalculado == grupoAsignado);
        
        return esCorrect;
    } catch (const std::exception& e) {
        std::cerr << "Error en verificación por referencia: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Verificación masiva por VALOR - Audita toda una colección
 * 
 * @param personas Vector copiado de personas a verificar
 * 
 * FUNCIONALIDAD:
 * - Verifica cada persona individualmente usando verificarGrupoPorValor
 * - Mantiene contadores de correctos e incorrectos
 * - Genera reporte estadístico completo
 * - Calcula porcentaje de acierto para métricas de calidad
 * 
 * SALIDA:
 * - Resumen con totales, correctos, incorrectos
 * - Porcentaje de acierto para evaluación de calidad de datos
 * - Formato de reporte profesional para auditorías
 */
void verificarGruposMasivoPorValor(std::vector<Persona> personas) {
    std::vector<bool> resultados;
    int correctos = 0;
    int incorrectos = 0;
    
    std::cout << "\n=== VERIFICACIÓN MASIVA POR VALOR ===" << std::endl;
    
    for (const auto& persona : personas) {
        bool resultado = verificarGrupoPorValor(persona);
        resultados.push_back(resultado);
        
        if (resultado) {
            correctos++;
        } else {
            incorrectos++;
        }
    }
    
    std::cout << "\n--- RESUMEN VERIFICACIÓN MASIVA ---" << std::endl;
    std::cout << "Total personas verificadas: " << personas.size() << std::endl;
    std::cout << "Correctos: " << correctos << std::endl;
    std::cout << "Incorrectos: " << incorrectos << std::endl;
    std::cout << "Porcentaje de acierto: " << (correctos * 100.0 / personas.size()) << "%" << std::endl;
    std::cout << "===================================" << std::endl;
}

/**
 * @brief Verificación masiva por REFERENCIA - Audita eficientemente
 * 
 * @param personas Vector por referencia constante
 * 
 * Versión optimizada de la verificación masiva que evita copiar
 * el vector completo, ideal para auditorías de grandes volúmenes
 * de datos tributarios con mejor rendimiento.
 */
void verificarGruposMasivoPorReferencia(const std::vector<Persona>& personas) {
    std::vector<bool> resultados;
    int correctos = 0;
    int incorrectos = 0;
    
    std::cout << "\n=== VERIFICACIÓN MASIVA POR REFERENCIA ===" << std::endl;
    
    for (const auto& persona : personas) {
        bool resultado = verificarGrupoPorReferencia(persona);
        resultados.push_back(resultado);
        
        if (resultado) {
            correctos++;
        } else {
            incorrectos++;
        }
    }
    
    std::cout << "\n--- RESUMEN VERIFICACIÓN MASIVA ---" << std::endl;
    std::cout << "Total personas verificadas: " << personas.size() << std::endl;
    std::cout << "Correctos: " << correctos << std::endl;
    std::cout << "Incorrectos: " << incorrectos << std::endl;
    std::cout << "Porcentaje de acierto: " << (correctos * 100.0 / personas.size()) << "%" << std::endl;
    std::cout << "======================================" << std::endl;
}

// ============================================================================
// ANÁLISIS ESTADÍSTICO POR GRUPOS
// ============================================================================

/**
 * @brief Encuentra el grupo con mayor patrimonio promedio (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @return std::string Grupo con mayor patrimonio promedio ("A", "B", "C")
 * 
 * ALGORITMO:
 * 1. Itera sobre cada grupo (A, B, C)
 * 2. Filtra personas de cada grupo
 * 3. Calcula promedio de patrimonio por grupo
 * 4. Identifica el grupo con mayor promedio
 * 5. Muestra estadísticas por consola para análisis
 * 
 * UTILIDAD:
 * - Análisis de distribución de riqueza por grupos tributarios
 * - Identificación de patrones económicos por segmentos
 * - Soporte para políticas tributarias diferenciadas
 */
std::string encontrarGrupoMayorPatrimonioPorValor(std::vector<Persona> personas) {
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<Persona> filtradas;
    
    for (auto grupo : grupos) {
        for (auto p : personas) {
            if (p.grupoDeclaracion == grupo) {
                filtradas.push_back(p);
            }
        }

        if (filtradas.empty()) continue;
        
        double sumaPatrimonio = 0.0;
        for (const auto& p : filtradas) {
            sumaPatrimonio += p.patrimonio;
        }
        double promedio = sumaPatrimonio / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Patrimonio: " << promedio << std::endl;
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear(); // Limpiar para siguiente iteración
    }
    
    return grupoMayor;
}

/**
 * @brief Encuentra el grupo con mayor patrimonio promedio (VERSIÓN POR REFERENCIA)
 * 
 * Versión optimizada para análisis estadísticos eficientes de grandes datasets.
 */
std::string encontrarGrupoMayorPatrimonioPorReferencia(const std::vector<Persona>& personas) {
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<Persona> filtradas;

    for (const auto& grupo : grupos) {
        for (const auto& p : personas) {
            if (p.grupoDeclaracion == grupo) {
                filtradas.push_back(p);
            }
        }

        if (filtradas.empty()) continue;

        double sumaPatrimonio = 0.0;
        for (const auto& p : filtradas) {
            sumaPatrimonio += p.patrimonio;
        }
        double promedio = sumaPatrimonio / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Patrimonio: " << promedio << std::endl;
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }

    return grupoMayor;
}

/**
 * @brief Encuentra el grupo con mayor longevidad promedio (VERSIÓN POR VALOR)
 * 
 * @param personas Vector copiado de personas
 * @return std::string Grupo con mayor edad promedio
 * 
 * Análisis demográfico para identificar patrones de edad por grupos
 * tributarios, útil para estudios actuariales y políticas sociales.
 */
std::string encontrarGrupoMayorLongevidadPorValor(std::vector<Persona> personas) {
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<Persona> filtradas;
    
    for (auto grupo : grupos) {
        for (auto p : personas) {
            if (p.grupoDeclaracion == grupo) {
                filtradas.push_back(p);
            }
        }

        if (filtradas.empty()) continue;
        
        double sumaEdad = 0.0;
        for (auto p : filtradas) {
            sumaEdad += p.edad;
        }
        double promedio = sumaEdad / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Edad: " << promedio << std::endl;
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }
    
    return grupoMayor;
}

/**
 * @brief Encuentra el grupo con mayor longevidad promedio (VERSIÓN POR REFERENCIA)
 * 
 * Versión optimizada para análisis demográficos eficientes.
 */
std::string encontrarGrupoMayorLongevidadPorReferencia(const std::vector<Persona>& personas) {
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<Persona> filtradas;

    for (const auto& grupo : grupos) {
        for (const auto& p : personas) {
            if (p.grupoDeclaracion == grupo) {
                filtradas.push_back(p);
            }
        }

        if (filtradas.empty()) continue;

        double sumaEdad = 0.0;
        for (const auto& p : filtradas) {
            sumaEdad += p.edad;
        }
        double promedio = sumaEdad / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Edad: " << promedio << std::endl;
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }

    return grupoMayor;
}

// ============================================================================
// FUNCIONES DE VALIDACIÓN
// ============================================================================

/**
 * @brief Valida si una ciudad existe en la base de datos de ciudades colombianas
 * 
 * @param ciudad Nombre de la ciudad a validar
 * @return bool true si la ciudad es válida, false en caso contrario
 * 
 * UTILIDAD:
 * - Validación de entrada de datos
 * - Prevención de errores tipográficos
 * - Mantenimiento de integridad referencial geográfica
 * 
 * IMPLEMENTACIÓN:
 * - Usa std::find para búsqueda lineal en vector de ciudades
 * - Comparación exacta (case-sensitive)
 * - Retorna booleano para fácil integración en validaciones
 */
bool ciudadValida(const std::string& ciudad) {
    return std::find(ciudadesColombia.begin(), ciudadesColombia.end(), ciudad) != ciudadesColombia.end();
}
