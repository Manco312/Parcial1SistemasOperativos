#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // std::mt19937, std::uniform_real_distribution
#include <vector>
#include <algorithm> // std::find_if

// ========================================================================
// BASES DE DATOS PARA GENERACIÓN REALISTA DE PERSONAS COLOMBIANAS
// ========================================================================
// Estas bases de datos contienen nombres, apellidos y ciudades comunes
// en Colombia para generar datos realistas y representativos del país.

// Nombres femeninos más populares en Colombia según registros civiles
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

// Nombres masculinos más populares en Colombia según registros civiles
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Nestor", "Gertridis"
};

// Apellidos más comunes en Colombia basados en estadísticas del DANE
const std::vector<std::string> apellidos = {
    "Gómez", "Rodríguez", "Martínez", "López", "García", "Pérez", "González", "Sánchez", "Ramírez", "Torres",
    "Díaz", "Vargas", "Castro", "Ruiz", "Álvarez", "Romero", "Suárez", "Rojas", "Moreno", "Muñoz", "Valencia",
};

// Principales ciudades colombianas ordenadas por población e importancia económica
const std::vector<std::string> ciudadesColombia = {
    "Bogotá", "Medellín", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Santa Marta", "Cúcuta", "Ibagué",
    "Manizales", "Pasto", "Neiva", "Villavicencio", "Armenia", "Sincelejo", "Valledupar", "Montería", "Popayán", "Tunja"
};

// ========================================================================
// FUNCIONES DE VALIDACIÓN Y UTILIDAD
// ========================================================================

/**
 * Valida si una ciudad pertenece al conjunto de ciudades colombianas válidas.
 * 
 * @param ciudad Nombre de la ciudad a validar
 * @return true si la ciudad es válida, false en caso contrario
 * 
 * COMPLEJIDAD: O(n) donde n es el número de ciudades
 * USO: Validación de entrada en formularios y APIs
 */
bool ciudadValida(const std::string& ciudad) {
    return std::find(ciudadesColombia.begin(), ciudadesColombia.end(), ciudad) != ciudadesColombia.end();
}

// ========================================================================
// GENERADORES DE DATOS ALEATORIOS
// ========================================================================

/**
 * Genera una fecha de nacimiento aleatoria en formato DD/MM/AAAA.
 * 
 * RANGO DE AÑOS: 1960-2009 (personas entre 16-65 años aproximadamente)
 * RANGO DE DÍAS: 1-28 (evita problemas con meses de febrero)
 * RANGO DE MESES: 1-12 (enero a diciembre)
 * 
 * @return String con fecha en formato "DD/MM/AAAA"
 * 
 * NOTA: Usa rand() simple, adecuado para datos de prueba no criptográficos
 */
std::string generarFechaNacimiento() {
    int dia = 1 + rand() % 28;       // Día: 1 a 28 (evita problemas con meses)
    int mes = 1 + rand() % 12;        // Mes: 1 a 12
    int anio = 1960 + rand() % 50;    // Año: 1960 a 2009
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

/**
 * Genera un ID único secuencial simulando números de cédula colombiana.
 * 
 * CARACTERÍSTICAS:
 * - Inicia en 1,000,000,000 (formato realista de cédula)
 * - Incremento secuencial garantiza unicidad
 * - Variable estática mantiene estado entre llamadas
 * 
 * @return String con el ID generado
 * 
 * THREAD-SAFETY: No es thread-safe debido a la variable estática
 */
std::string generarID() {
    static long contador = 1000000000; // Inicia en 1,000,000,000
    return std::to_string(contador++); // Convierte a string e incrementa
}

/**
 * Genera números decimales aleatorios de alta calidad en un rango específico.
 * 
 * VENTAJAS SOBRE rand():
 * - Mersenne Twister: mejor distribución estadística
 * - Distribución uniforme real: no sesgos
 * - Semilla basada en tiempo: mejor aleatoriedad
 * 
 * @param min Valor mínimo (inclusivo)
 * @param max Valor máximo (inclusivo)
 * @return Número decimal aleatorio en el rango [min, max]
 * 
 * USO: Valores monetarios, porcentajes, mediciones precisas
 */
double randomDouble(double min, double max) {
    static std::mt19937 generator(time(nullptr)); // Semilla basada en tiempo
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

// ========================================================================
// GENERACIÓN DE PERSONAS Y COLECCIONES
// ========================================================================

/**
 * Genera una persona completa con datos aleatorios pero realistas.
 * 
 * PROCESO DE GENERACIÓN:
 * 1. Determina género aleatoriamente (50/50)
 * 2. Selecciona nombre según género de las bases de datos
 * 3. Construye apellido compuesto (dos apellidos aleatorios)
 * 4. Genera ID secuencial único
 * 5. Asigna ciudad aleatoria de Colombia
 * 6. Calcula grupo de declaración basado en últimos 2 dígitos del ID
 * 7. Genera datos financieros realistas con correlaciones lógicas
 * 
 * LÓGICA DE GRUPOS:
 * - Grupo A: ID termina en 00-39 (40% de la población)
 * - Grupo B: ID termina en 40-79 (40% de la población)  
 * - Grupo C: ID termina en 80-99 (20% de la población)
 * 
 * DATOS FINANCIEROS REALISTAS:
 * - Ingresos: 10M-500M COP (rango clase media-alta colombiana)
 * - Patrimonio: 0-2,000M COP (incluye desde sin patrimonio hasta muy ricos)
 * - Deudas: máximo 70% del patrimonio (límite realista de endeudamiento)
 * - Declarante: 70% probabilidad si ingresos > 50M COP
 * 
 * @return Objeto Persona completamente inicializado
 */
Persona generarPersona() {
    // Decide si es hombre o mujer (50% probabilidad cada uno)
    bool esHombre = rand() % 2;
    
    // Selecciona nombre según género de las bases de datos correspondientes
    std::string nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    // Construye apellido compuesto (tradición colombiana de dos apellidos)
    std::string apellido = apellidos[rand() % apellidos.size()];
    apellido += " ";
    apellido += apellidos[rand() % apellidos.size()];
    
    // Genera identificación y ubicación
    std::string id = generarID();
    std::string ciudad = ciudadesColombia[rand() % ciudadesColombia.size()];
    std::string fecha = generarFechaNacimiento();

    // Calcula grupo de declaración basado en los últimos 2 dígitos del ID
    int ultDigitos = std::stoi(id.substr(id.length() - 2));
    std::string grupo;

    // Asigna el grupo según las reglas establecidas
    if (ultDigitos >= 0 && ultDigitos <= 39) {
        grupo = "A";        // 40% de la población
    } else if (ultDigitos >= 40 && ultDigitos <= 79) {
        grupo = "B";        // 40% de la población
    } else {
        grupo = "C";        // 20% de la población
    }

    // Calcula edad aproximada basada en el año de nacimiento
    int edad = 2025 - std::stoi(fecha.substr(fecha.find_last_of('/') + 1));

    // Genera datos financieros con correlaciones realistas
    double ingresos = randomDouble(10000000, 500000000);   // 10M a 500M COP
    double patrimonio = randomDouble(0, 2000000000);       // 0 a 2,000M COP
    double deudas = randomDouble(0, patrimonio * 0.7);     // Deudas hasta el 70% del patrimonio
    
    // Lógica de declarante: mayor probabilidad si ingresos altos
    bool declarante = (ingresos > 50000000) && (rand() % 100 > 30); // 70% probabilidad si ingresos > 50M
    
    return Persona(nombre, apellido, id, ciudad, fecha, grupo, edad, ingresos, patrimonio, deudas, declarante);
}

/**
 * Genera una colección de n personas con optimización de memoria.
 * 
 * OPTIMIZACIONES:
 * - reserve(n): pre-asigna memoria para evitar realocaciones
 * - push_back: construcción eficiente de objetos
 * 
 * @param n Número de personas a generar
 * @return Vector con n personas generadas aleatoriamente
 * 
 * COMPLEJIDAD: O(n) tiempo, O(n) espacio
 * USO: Creación de datasets para pruebas, simulaciones, benchmarks
 */
std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    personas.reserve(n); // Reserva espacio para n personas (eficiencia)
    
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

// ========================================================================
// FUNCIONES DE BÚSQUEDA Y CONSULTA
// ========================================================================

/**
 * Busca una persona por su ID en una colección usando búsqueda lineal.
 * 
 * ALGORITMO: Búsqueda secuencial con lambda y find_if
 * COMPLEJIDAD: O(n) en el peor caso
 * RETORNO: Puntero a la persona encontrada o nullptr si no existe
 * 
 * @param personas Vector de personas donde buscar
 * @param id ID de la persona a buscar
 * @return Puntero constante a la persona encontrada o nullptr
 * 
 * VENTAJAS:
 * - Retorna puntero: evita copias innecesarias
 * - Const correctness: no modifica los datos
 * - nullptr: indicador claro de "no encontrado"
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    // Usa find_if con lambda para búsqueda eficiente
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.getId() == id; });
    
    if (it != personas.end()) {
        return &(*it); // Devuelve puntero a la persona encontrada
    } else {
        return nullptr; // No encontrado
    }
}

// ========================================================================
// BÚSQUEDAS DE PERSONA MÁS LONGEVA
// ========================================================================

/**
 * Busca la persona más longeva (VERSIÓN POR VALOR).
 * 
 * CARACTERÍSTICAS:
 * - Recibe vector por valor (copia completa)
 * - Retorna objeto Persona (copia del resultado)
 * - Usa max_element con lambda comparador
 * 
 * VENTAJAS: Datos inmutables, thread-safe
 * DESVENTAJAS: Alto costo de memoria (2 copias completas)
 * 
 * @param personas Vector de personas (copiado)
 * @return Persona más longeva (copia)
 * 
 * COMPLEJIDAD: O(n) tiempo, O(n) espacio adicional
 */
Persona buscarMasLongevoPorValor(std::vector<Persona> personas) {
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.getEdad() < b.getEdad(); });
}

/**
 * Busca la persona más longeva (VERSIÓN POR REFERENCIA).
 * 
 * CARACTERÍSTICAS:
 * - Recibe vector por referencia constante (sin copia)
 * - Retorna puntero constante (sin copia del resultado)
 * - Validación de vector vacío
 * 
 * VENTAJAS: Eficiente en memoria, rápido
 * DESVENTAJAS: Puntero puede invalidarse si vector se modifica
 * 
 * @param personas Vector de personas (referencia constante)
 * @return Puntero a la persona más longeva o nullptr si vacío
 * 
 * COMPLEJIDAD: O(n) tiempo, O(1) espacio adicional
 */
const Persona* buscarMasLongevoPorReferencia(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;

    auto it = std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.getEdad() < b.getEdad(); });

    return &(*it);
}

/**
 * Busca la persona más longeva en una ciudad específica (VERSIÓN POR VALOR).
 * 
 * PROCESO:
 * 1. Filtra personas de la ciudad especificada
 * 2. Valida que existan personas en esa ciudad
 * 3. Busca el más longevo entre los filtrados
 * 
 * @param personas Vector de personas (copiado)
 * @param ciudad Ciudad donde buscar
 * @return Persona más longeva de la ciudad
 * @throws std::runtime_error si no hay personas en la ciudad
 * 
 * COMPLEJIDAD: O(n) tiempo, O(k) espacio donde k = personas en la ciudad
 */
Persona buscarMasLongevoPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad) {
    // Filtrar solo personas de la ciudad especificada
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.getCiudadNacimiento() == ciudad) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más longevo en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.getEdad() < b.getEdad(); });
}

/**
 * Busca la persona más longeva en una ciudad específica (VERSIÓN POR REFERENCIA).
 * 
 * OPTIMIZACIÓN: Usa vector de punteros para evitar copiar objetos Persona
 * 
 * @param personas Vector de personas (referencia constante)
 * @param ciudad Ciudad donde buscar
 * @return Puntero a la persona más longeva de la ciudad
 * @throws std::runtime_error si no hay personas en la ciudad
 * 
 * VENTAJA: Más eficiente en memoria que la versión por valor
 */
const Persona* buscarMasLongevoPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad) {
    // Filtrar usando punteros para mayor eficiencia
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.getCiudadNacimiento() == ciudad) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más longevo usando comparación de punteros
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->getEdad() < b->getEdad(); });
}

// ========================================================================
// BÚSQUEDAS DE PERSONA CON MAYOR PATRIMONIO
// ========================================================================

/**
 * Busca la persona con mayor patrimonio (VERSIÓN POR VALOR).
 * Similar a buscarMasLongevoPorValor pero comparando patrimonio.
 */
Persona buscarMasPatrimonioPorValor(std::vector<Persona> personas) {
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.getPatrimonio() < b.getPatrimonio(); });
}

/**
 * Busca la persona con mayor patrimonio (VERSIÓN POR REFERENCIA).
 * Versión eficiente que evita copias de memoria.
 */
const Persona* buscarMasPatrimonioPorReferencia(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;

    auto it = std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.getPatrimonio() < b.getPatrimonio(); });

    return &(*it);
}

/**
 * Busca la persona con mayor patrimonio en una ciudad específica (VERSIÓN POR VALOR).
 * 
 * PROCESO:
 * 1. Filtra personas de la ciudad especificada
 * 2. Valida que existan personas en esa ciudad
 * 3. Busca el más rico entre los filtrados
 * 
 * @param personas Vector de personas (copiado)
 * @param ciudad Ciudad donde buscar
 * @return Persona más rica de la ciudad
 * @throws std::runtime_error si no hay personas en la ciudad
 * 
 * COMPLEJIDAD: O(n) tiempo, O(k) espacio donde k = personas en la ciudad
 */
Persona buscarMasPatrimonioPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad) {
    // Filtrar solo personas de la ciudad especificada
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.getCiudadNacimiento() == ciudad) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más rico en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.getPatrimonio() < b.getPatrimonio(); });
}

/**
 * Busca la persona con mayor patrimonio en una ciudad específica (VERSIÓN POR REFERENCIA).
 * 
 * OPTIMIZACIÓN: Usa vector de punteros para evitar copiar objetos Persona
 * 
 * @param personas Vector de personas (referencia constante)
 * @param ciudad Ciudad donde buscar
 * @return Puntero a la persona más rica de la ciudad
 * @throws std::runtime_error si no hay personas en la ciudad
 * 
 * VENTAJA: Más eficiente en memoria que la versión por valor
 */
const Persona* buscarMasPatrimonioPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad) {
    // Filtrar usando punteros para mayor eficiencia
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.getCiudadNacimiento() == ciudad) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más rico usando comparación de punteros
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->getPatrimonio() < b->getPatrimonio(); });
}

/**
 * Busca la persona con mayor patrimonio en un grupo específico (VERSIÓN POR VALOR).
 * 
 * PROCESO:
 * 1. Filtra personas del grupo especificado
 * 2. Valida que existan personas en ese grupo
 * 3. Busca el más rico entre los filtrados
 * 
 * @param personas Vector de personas (copiado)
 * @param grupo Grupo de declaración donde buscar
 * @return Persona más rica del grupo
 * @throws std::runtime_error si no hay personas en el grupo
 * 
 * COMPLEJIDAD: O(n) tiempo, O(k) espacio donde k = personas en el grupo
 */
Persona buscarMasPatrimonioPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo) {
    // Filtrar solo personas del grupo especificado
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.getGrupoDeclaracion() == grupo) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en el grupo: " + grupo);
    }

    // Buscar el más rico en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.getPatrimonio() < b.getPatrimonio(); });
}

/**
 * Busca la persona con mayor patrimonio en un grupo específico (VERSIÓN POR REFERENCIA).
 * 
 * OPTIMIZACIÓN: Usa vector de punteros para evitar copiar objetos Persona
 * 
 * @param personas Vector de personas (referencia constante)
 * @param grupo Grupo de declaración donde buscar
 * @return Puntero a la persona más rica del grupo
 * @throws std::runtime_error si no hay personas en el grupo
 * 
 * VENTAJA: Más eficiente en memoria que la versión por valor
 */
const Persona* buscarMasPatrimonioPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo) {
    // Filtrar usando punteros para mayor eficiencia
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.getGrupoDeclaracion() == grupo) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en el grupo: " + grupo);
    }

    // Buscar el más rico usando comparación de punteros
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->getPatrimonio() < b->getPatrimonio(); });
}

/**
 * Lista y cuenta personas de un grupo (por valor).
 * 
 * @param personas  Vector de personas.
 * @param grupo     Grupo de declaración a filtrar (ej. "A", "B", "C").
 * @return Vector con las personas del grupo especificado.
 */
std::vector<Persona> listarPersonasPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo) {
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.getGrupoDeclaracion() == grupo) {
            p.mostrarResumen();
            filtradas.push_back(p);
        }
    }
    return filtradas;
}

/**
 * Lista y cuenta personas de un grupo (por referencia).
 * 
 * @param personas  Vector de personas.
 * @param grupo     Grupo de declaración a filtrar.
 * @return Vector de punteros a personas del grupo especificado.
 */
std::vector<const Persona*> listarPersonasPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo) {
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.getGrupoDeclaracion() == grupo) {
            p.mostrarResumen();
            filtradas.push_back(&p);
        }
    }
    return filtradas;
}

// ========================================================================
// FUNCIONES DE ANÁLISIS Y ESTADÍSTICAS
// ========================================================================

/**
 * Calcula el grupo de declaración correcto basado en los últimos dígitos de la cédula.
 * 
 * REGLAS DE ASIGNACIÓN:
 * - Últimos 2 dígitos 00-39: Grupo A (40% población)
 * - Últimos 2 dígitos 40-79: Grupo B (40% población)
 * - Últimos 2 dígitos 80-99: Grupo C (20% población)
 * 
 * @param cedula Número de cédula como string
 * @return Grupo calculado ("A", "B", o "C")
 * @throws std::invalid_argument si la cédula tiene menos de 2 dígitos
 * 
 * USO: Verificación de consistencia de datos, auditorías
 */
std::string calcularGrupoCorrectoPorCedula(const std::string& cedula) {
    if (cedula.length() < 2) {
        throw std::invalid_argument("La cédula debe tener al menos 2 dígitos");
    }
    
    // Extraer los últimos 2 dígitos
    int ultDigitos = std::stoi(cedula.substr(cedula.length() - 2));
    
    // Aplicar las reglas de asignación
    if (ultDigitos >= 0 && ultDigitos <= 39) {
        return "A";
    } else if (ultDigitos >= 40 && ultDigitos <= 79) {
        return "B";
    } else { // 80-99
        return "C";
    }
}

// ========================================================================
// FUNCIONES DE VERIFICACIÓN DE INTEGRIDAD
// ========================================================================

/**
 * Verifica si el grupo asignado coincide con el calculado (VERSIÓN POR VALOR).
 * 
 * PROCESO:
 * 1. Recibe copia de la persona (inmutable)
 * 2. Calcula grupo correcto basado en cédula
 * 3. Compara con grupo asignado
 * 4. Maneja excepciones de forma segura
 * 
 * @param persona Persona a verificar (copia)
 * @return true si el grupo es correcto, false en caso contrario
 * 
 * VENTAJAS: Thread-safe, no afecta datos originales
 * DESVENTAJAS: Costo de copia del objeto
 */
bool verificarGrupoPorValor(Persona persona) {
    try {
        std::string grupoCalculado = calcularGrupoCorrectoPorCedula(persona.getId());
        std::string grupoAsignado = persona.getGrupoDeclaracion();
        
        bool esCorrect = (grupoCalculado == grupoAsignado);
        
        return esCorrect;
    } catch (const std::exception& e) {
        std::cerr << "Error en verificación por valor: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Verifica si el grupo asignado coincide con el calculado (VERSIÓN POR REFERENCIA).
 * 
 * OPTIMIZACIÓN: Usa referencia constante para evitar copia del objeto.
 * Ideal para verificaciones masivas o objetos grandes.
 * 
 * @param persona Referencia constante a la persona
 * @return true si el grupo es correcto, false en caso contrario
 * 
 * COMPLEJIDAD: O(1) tiempo, O(1) espacio adicional
 */
bool verificarGrupoPorReferencia(const Persona& persona) {
    try {
        std::string grupoCalculado = calcularGrupoCorrectoPorCedula(persona.getId());
        std::string grupoAsignado = persona.getGrupoDeclaracion();
        
        bool esCorrect = (grupoCalculado == grupoAsignado);
        
        return esCorrect;
    } catch (const std::exception& e) {
        std::cerr << "Error en verificación por referencia: " << e.what() << std::endl;
        return false;
    }
}

// ========================================================================
// FUNCIONES DE AUDITORÍA MASIVA
// ========================================================================

/**
 * Realiza verificación masiva de grupos en toda una colección (POR VALOR).
 * 
 * CARACTERÍSTICAS:
 * - Procesa cada persona individualmente
 * - Genera estadísticas completas de verificación
 * - Muestra resumen detallado con porcentajes
 * 
 * @param personas Vector de personas a verificar (copiado)
 * 
 * SALIDA: Imprime estadísticas detalladas en consola
 * USO: Auditorías de calidad de datos, validación de sistemas
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
 * Realiza verificación masiva de grupos en toda una colección (POR REFERENCIA).
 * 
 * OPTIMIZACIÓN: Versión eficiente para grandes volúmenes de datos.
 * Misma funcionalidad que la versión por valor pero sin copias costosas.
 * 
 * @param personas Vector de personas (referencia constante)
 * 
 * VENTAJA: Significativamente más rápido para datasets grandes
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

// ========================================================================
// ANÁLISIS ESTADÍSTICO POR GRUPOS
// ========================================================================

/**
 * Encuentra el grupo con mayor patrimonio promedio (VERSIÓN POR VALOR).
 * 
 * ALGORITMO:
 * 1. Itera sobre cada grupo (A, B, C)
 * 2. Filtra personas de cada grupo
 * 3. Calcula promedio de patrimonio
 * 4. Determina el grupo con mayor promedio
 * 
 * @param personas Vector de personas (copiado)
 * @return String con el grupo que tiene mayor patrimonio promedio
 * 
 * SALIDA: Imprime promedios de cada grupo para análisis
 * USO: Análisis socioeconómico, segmentación de mercado
 */
std::string encontrarGrupoMayorPatrimonioPorValor(std::vector<Persona> personas) {
    
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<Persona> filtradas;
    
    for (auto grupo : grupos) {
        // Filtrar personas del grupo actual
        for (auto p : personas) {
            if (p.getGrupoDeclaracion() == grupo) {
                filtradas.push_back(p);
            }
        }

        if (filtradas.empty()) continue;
        
        // Calcular promedio de patrimonio del grupo
        double sumaPatrimonio = 0.0;
        for (const auto& p : filtradas) {
            sumaPatrimonio += p.getPatrimonio();
        }
        double promedio = sumaPatrimonio / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Patrimonio: " << promedio << std::endl;
        
        // Actualizar grupo con mayor promedio
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }
    
    return grupoMayor;
}

/**
 * Encuentra el grupo con mayor patrimonio promedio (VERSIÓN POR REFERENCIA).
 * 
 * ALGORITMO:
 * 1. Itera sobre cada grupo (A, B, C)
 * 2. Filtra personas de cada grupo usando punteros
 * 3. Calcula promedio de patrimonio
 * 4. Determina el grupo con mayor promedio
 * 
 * @param personas Vector de personas (referencia constante)
 * @return String con el grupo que tiene mayor patrimonio promedio
 * 
 * SALIDA: Imprime promedios de cada grupo para análisis
 * USO: Análisis socioeconómico, segmentación de mercado
 */
std::string encontrarGrupoMayorPatrimonioPorReferencia(const std::vector<Persona>& personas) {
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<const Persona*> filtradas;

    for (const auto& grupo : grupos) {
        // Filtrar personas del grupo actual usando punteros
        for (const auto& p : personas) {
            if (p.getGrupoDeclaracion() == grupo) {
                filtradas.push_back(&p);
            }
        }

        if (filtradas.empty()) continue;

        // Calcular promedio de patrimonio del grupo
        double sumaPatrimonio = 0.0;
        for (const auto& p : filtradas) {
            sumaPatrimonio += p->getPatrimonio();
        }
        double promedio = sumaPatrimonio / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Patrimonio: " << promedio << std::endl;

        // Actualizar grupo con mayor promedio
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }

    return grupoMayor;
}

/**
 * Encuentra el grupo con mayor longevidad promedio (VERSIÓN POR VALOR).
 * 
 * ALGORITMO:
 * 1. Itera sobre cada grupo (A, B, C)
 * 2. Filtra personas de cada grupo
 * 3. Calcula promedio de edad
 * 4. Determina el grupo con mayor promedio
 * 
 * @param personas Vector de personas (copiado)
 * @return String con el grupo que tiene mayor longevidad promedio
 * 
 * SALIDA: Imprime promedios de cada grupo para análisis
 * USO: Análisis socioeconómico, segmentación de mercado
 */
std::string encontrarGrupoMayorLongevidadPorValor(std::vector<Persona> personas) {
    
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<Persona> filtradas;
    for (auto grupo : grupos) {
        // Filtrar personas del grupo actual
        for (auto p : personas) {
            if (p.getGrupoDeclaracion() == grupo) {
                filtradas.push_back(p);
            }
        }

        if (filtradas.empty()) continue;
        
        // Calcular promedio de edad del grupo
        double sumaEdad = 0.0;
        for (const auto& p : filtradas) {
            sumaEdad += p.getEdad();
        }
        double promedio = sumaEdad / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Edad: " << promedio << std::endl;
        
        // Actualizar grupo con mayor promedio
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }
    
    return grupoMayor;
}

/**
 * Encuentra el grupo con mayor longevidad promedio (VERSIÓN POR REFERENCIA).
 * 
 * ALGORITMO:
 * 1. Itera sobre cada grupo (A, B, C)
 * 2. Filtra personas de cada grupo usando punteros
 * 3. Calcula promedio de edad
 * 4. Determina el grupo con mayor promedio
 * 
 * @param personas Vector de personas (referencia constante)
 * @return String con el grupo que tiene mayor longevidad promedio
 * 
 * SALIDA: Imprime promedios de cada grupo para análisis
 * USO: Análisis socioeconómico, segmentación de mercado
 */
std::string encontrarGrupoMayorLongevidadPorReferencia(const std::vector<Persona>& personas) {
    std::vector<std::string> grupos = {"A", "B", "C"};
    std::string grupoMayor;
    double mayorPromedio = 0.0;
    std::vector<const Persona*> filtradas;

    for (const auto& grupo : grupos) {
        // Filtrar personas del grupo actual usando punteros
        for (const auto& p : personas) {
            if (p.getGrupoDeclaracion() == grupo) {
                filtradas.push_back(&p);
            }
        }

        if (filtradas.empty()) continue;

        // Calcular promedio de edad del grupo
        double sumaEdad = 0.0;
        for (const auto& p : filtradas) {
            sumaEdad += p->getEdad();
        }
        double promedio = sumaEdad / filtradas.size();

        std::cout << "Grupo " << grupo << " - Promedio Edad: " << promedio << std::endl;

        // Actualizar grupo con mayor promedio
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            grupoMayor = grupo;
        }
        filtradas.clear();
    }

    return grupoMayor;
}
