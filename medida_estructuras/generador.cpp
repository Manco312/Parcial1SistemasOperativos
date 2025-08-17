#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // Generadores aleatorios modernos
#include <vector>
#include <algorithm> // Para find_if
#include <iostream>  // Para std::cout
#include <stdexcept> // Para std::runtime_error y std::invalid_argument

// --- Bases de datos para generación realista ---

// Nombres femeninos comunes en Colombia
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

// Nombres masculinos comunes en Colombia
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Sofia","Camila","Juana","Ana","Laura","Karla","Andrea","Daniela","Alejandra","Martina",
    "Nelly","María","Nestor","Trinidad","Fernanda", "Carolina", "Lina", "Gertridis"
};

// Apellidos comunes en Colombia
const std::vector<std::string> apellidos = {
    "Gómez", "Rodríguez", "Martínez", "López", "García", "Pérez", "González", "Sánchez", "Ramírez", "Torres",
    "Díaz", "Vargas", "Castro", "Ruiz", "Álvarez", "Romero", "Suárez", "Rojas", "Moreno", "Muñoz", "Valencia",
};

// Principales ciudades colombianas
const std::vector<std::string> ciudadesColombia = {
    "Bogotá", "Medellín", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Santa Marta", "Cúcuta", "Ibagué",
    "Manizales", "Pasto", "Neiva", "Villavicencio", "Armenia", "Sincelejo", "Valledupar", "Montería", "Popayán", "Tunja"
};

// Implementación de funciones generadoras

std::string generarFechaNacimiento() {
    // Genera día aleatorio (1-28 para simplificar)
    int dia = 1 + rand() % 28;
    // Mes aleatorio (1-12)
    int mes = 1 + rand() % 12;
    // Año entre 1960-2010
    int anio = 1960 + rand() % 50;
    
    // Convierte a string en formato DD/MM/AAAA
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

std::string generarID() {
    static long contador = 1000000000; // ID inicial
    return std::to_string(contador++); // Incrementa después de usar
}

double randomDouble(double min, double max) {
    // Generador moderno Mersenne Twister
    static std::mt19937 generator(time(nullptr));
    // Distribución uniforme en rango [min, max]
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

Persona generarPersona() {
    Persona p; // Crea una instancia de la estructura Persona
    
    // Decide aleatoriamente si es hombre o mujer
    bool esHombre = rand() % 2;
    
    // Selecciona nombre según género
    p.nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    // Combina dos apellidos aleatorios
    p.apellido = apellidos[rand() % apellidos.size()] + " " + 
                 apellidos[rand() % apellidos.size()];
    
    // Genera identificadores únicos
    p.id = generarID();
    // Ciudad aleatoria de Colombia
    p.ciudadNacimiento = ciudadesColombia[rand() % ciudadesColombia.size()];
    // Fecha aleatoria
    p.fechaNacimiento = generarFechaNacimiento();

    p.edad = 2025 - std::stoi(p.fechaNacimiento.substr(p.fechaNacimiento.find_last_of('/') + 1));
    
    // --- Generación de datos económicos realistas ---    
    // Ingresos entre 10 millones y 500 millones COP
    p.ingresosAnuales = randomDouble(10000000, 500000000);
    // Patrimonio entre 0 y 2 mil millones COP
    p.patrimonio = randomDouble(0, 2000000000);
    // Deudas hasta el 70% del patrimonio
    p.deudas = randomDouble(0, p.patrimonio * 0.7);
    // 70% probabilidad de ser declarante si gana > 50 millones
    p.declaranteRenta = (p.ingresosAnuales > 50000000) && (rand() % 100 > 30);
    
    // Calcular grupo de declaración basado en la cédula
    p.grupoDeclaracion = calcularGrupoCorrectoPorCedula(p.id);
    
    return p; // Retorna la estructura completa
}

std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    personas.reserve(n); // Reserva espacio para n personas (eficiencia)
    
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    // Usa find_if con una lambda para buscar por ID
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.id == id; }); // Direct field access for structures
    
    if (it != personas.end()) {
        return &(*it); // Devuelve puntero a la persona encontrada
    } else {
        return nullptr; // No encontrado
    }
}

// --- Versión por valor ---
Persona buscarMasLongevoPorValor(std::vector<Persona> personas) {
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.edad < b.edad; });
}

// --- Versión por referencia ---
const Persona* buscarMasLongevoPorReferencia(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;

    auto it = std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.edad < b.edad; });

    return &(*it);
}

// --- Versión por valor en ciudad ---
Persona buscarMasLongevoPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad) {
    // Filtrar solo personas de la ciudad especificada
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más longevo en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.edad < b.edad; });
}

// --- Versión por referencia en ciudad ---
const Persona* buscarMasLongevoPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad) {
    // Filtrar solo personas de la ciudad especificada
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más longevo en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->edad < b->edad; });
}

// --- Versión por valor ---
Persona buscarMasPatrimonioPorValor(std::vector<Persona> personas) {
    return *std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });
}

// --- Versión por referencia ---
const Persona* buscarMasPatrimonioPorReferencia(const std::vector<Persona>& personas) {
    if (personas.empty()) return nullptr;

    auto it = std::max_element(personas.begin(), personas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });

    return &(*it);
}

// --- Versión por valor en ciudad ---
Persona buscarMasPatrimonioPorValorEnCiudad(std::vector<Persona> personas, const std::string& ciudad) {
    // Filtrar solo personas de la ciudad especificada
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más rico en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });
}

// --- Versión por referencia en ciudad ---
const Persona* buscarMasPatrimonioPorReferenciaEnCiudad(const std::vector<Persona>& personas, const std::string& ciudad) {
    // Filtrar solo personas de la ciudad especificada
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.ciudadNacimiento == ciudad) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en la ciudad: " + ciudad);
    }

    // Buscar el más rico en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->patrimonio < b->patrimonio; });
}

// --- Versión por valor en grupo ---
Persona buscarMasPatrimonioPorValorEnGrupo(std::vector<Persona> personas, const std::string& grupo) {
    // Filtrar solo personas del grupo especificado
    std::vector<Persona> filtradas;
    for (const auto& p : personas) {
        if (p.grupoDeclaracion == grupo) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en el grupo: " + grupo);
    }

    // Buscar el más rico en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona& a, const Persona& b) { return a.patrimonio < b.patrimonio; });
}

// --- Versión por referencia en grupo ---
const Persona* buscarMasPatrimonioPorReferenciaEnGrupo(const std::vector<Persona>& personas, const std::string& grupo) {
    // Filtrar solo personas del grupo especificado
    std::vector<const Persona*> filtradas;
    for (const auto& p : personas) {
        if (p.grupoDeclaracion == grupo) {
            filtradas.push_back(&p);
        }
    }

    if (filtradas.empty()) {
        throw std::runtime_error("No hay personas registradas en el grupo: " + grupo);
    }

    // Buscar el más rico en las filtradas
    return *std::max_element(filtradas.begin(), filtradas.end(),
        [](const Persona* a, const Persona* b) { return a->patrimonio < b->patrimonio; });
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
        if (p.grupoDeclaracion == grupo) {
            // Note: For structures, you'd need to implement a display function or print manually
            std::cout << "ID: " << p.id << ", Nombre: " << p.nombre << " " << p.apellido << std::endl;
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
        if (p.grupoDeclaracion == grupo) {
            std::cout << "ID: " << p.id << ", Nombre: " << p.nombre << " " << p.apellido << std::endl;
            filtradas.push_back(&p);
        }
    }
    return filtradas;
}

/**
 * Función para calcular el grupo correcto basado en la cédula
 * 
 * POR QUÉ: Determinar el grupo de declaración correcto según los últimos dígitos
 * CÓMO: Extrae los últimos 2 dígitos y aplica las reglas de asignación
 * PARA QUÉ: Verificar la consistencia de los datos
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

/**
 * Verificación por VALOR - Recibe una copia de la persona
 * 
 * POR QUÉ: Verificar si el grupo asignado coincide con el calculado por cédula
 * CÓMO: Copia la persona, calcula el grupo correcto y compara
 * PARA QUÉ: Validación de datos sin modificar el original
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
 * Verificación por REFERENCIA - Recibe un puntero/referencia a la persona
 * 
 * POR QUÉ: Verificar sin copiar el objeto (más eficiente)
 * CÓMO: Usa referencia constante, calcula el grupo y compara
 * PARA QUÉ: Validación eficiente para objetos grandes o muchas verificaciones
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
 * Verificación masiva por VALOR - Verifica toda una colección
 * 
 * POR QUÉ: Validar múltiples personas de una vez
 * CÓMO: Itera sobre copias de las personas y verifica cada una
 * PARA QUÉ: Auditoría completa de datos
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
 * Verificación masiva por REFERENCIA - Verifica toda una colección
 * 
 * POR QUÉ: Validar múltiples personas eficientemente
 * CÓMO: Itera sobre referencias a las personas sin copiarlas
 * PARA QUÉ: Auditoría eficiente de grandes volúmenes de datos
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

/**
 * Encuentra el grupo con mayor patrimonio promedio (por valor).
 * 
 * POR QUÉ: Determinar qué grupo de declaración tiene mejor situación económica.
 * CÓMO: Calcula el promedio de patrimonio por grupo y encuentra el máximo.
 * PARA QUÉ: Análisis estadístico de grupos de declaración.
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
        filtradas.clear();
    }
    
    return grupoMayor;
}

/**
 * Encuentra el grupo con mayor patrimonio promedio (por referencia).
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
 * Encuentra el grupo con mayor longevidad promedio (por valor).
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
 * Encuentra el grupo con mayor longevidad promedio (por referencia).
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

bool ciudadValida(const std::string& ciudad) {
    return std::find(ciudadesColombia.begin(), ciudadesColombia.end(), ciudad) != ciudadesColombia.end();
}
