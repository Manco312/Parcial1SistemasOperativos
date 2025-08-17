#include <iostream>
#include <vector>
#include <limits> // Para manejo de límites de entrada
#include <memory> // Para std::unique_ptr y std::make_unique
#include "persona.h"
#include "generador.h"
#include "monitor.h" // Nuevo header para monitoreo

void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Crear nuevo conjunto de datos.";
    std::cout << "\n1. Mostrar resumen de todas las personas.";
    std::cout << "\n2. Mostrar detalle completo por índice.";
    std::cout << "\n3. Buscar persona por ID.";
    std::cout << "\n4. Buscar persona mas longeva por valor.";
    std::cout << "\n5. Buscar persona mas longeva por referencia.";
    std::cout << "\n6. Buscar persona con mas patrimonio por valor.";
    std::cout << "\n7. Buscar persona con mas patrimonio por referencia.";
    std::cout << "\n8. Listar personas por grupo (A, B o C) por valor.";
    std::cout << "\n9. Listar personas por grupo (A, B o C) por referencia.";
    std::cout << "\n10. Verificar grupos por valor.";
    std::cout << "\n11. Verificar grupos por referencia.";
    std::cout << "\n12. Encontrar grupo con mayor patrimonio en promedio por valor.";
    std::cout << "\n13. Encontrar grupo con mayor patrimonio en promedio por referencia.";
    std::cout << "\n14. Encontrar grupo con mayor longevidad en promedio por valor.";
    std::cout << "\n15. Encontrar grupo con mayor longevidad en promedio por referencia.";
    std::cout << "\n16. Mostrar estadísticas de rendimiento.";
    std::cout << "\n17. Exportar estadísticas a CSV.";
    std::cout << "\n18. Salir.";
    std::cout << "\nSeleccione una opción: ";
}

int main() {
    srand(time(nullptr));
    
    // Usar unique_ptr para manejar la colección de personas
    std::unique_ptr<std::vector<Persona>> personas = nullptr;
    Monitor monitor;
    
    std::string opcionString;
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcionString;
        
        try {
            opcion = std::stoi(opcionString);
        } catch (const std::invalid_argument&) {
            std::cout << "Entrada inválida. Por favor, ingrese un número.\n";
            continue; // Vuelve a mostrar el menú
        }
        
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        
        // Iniciar medición de tiempo y memoria para esta operación
        long memoria_inicio = monitor.obtener_memoria();
        
        switch(opcion) {
            case 0: {
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;
                
                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }
                
                monitor.iniciar_tiempo();
                
                // Generar el nuevo conjunto de datos
                personas = std::make_unique<std::vector<Persona>>(generarColeccion(n));
                tam = personas->size();
                
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;
                
                std::cout << "Generadas " << tam << " personas en " 
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";
                
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }
                
            case 1: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                monitor.iniciar_tiempo();
                
                tam = personas->size();
                std::cout << "\n=== RESUMEN DE PERSONAS (" << tam << ") ===\n";
                for(size_t i = 0; i < tam; ++i) {
                    std::cout << i << ". ";
                    (*personas)[i].mostrarResumen();
                    std::cout << "\n";
                }
                
                double tiempo_mostrar = monitor.detener_tiempo();
                long memoria_mostrar = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_mostrar << " ms, Memoria: " << memoria_mostrar << " KB\n";
                monitor.registrar("Mostrar resumen", tiempo_mostrar, memoria_mostrar);
                break;
            }
                
            case 2: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                monitor.iniciar_tiempo();
                
                tam = personas->size();
                std::cout << "\nIngrese el índice (0-" << tam-1 << "): ";
                if(std::cin >> indice) {
                    if(indice >= 0 && static_cast<size_t>(indice) < tam) {
                        (*personas)[indice].mostrar();
                    } else {
                        std::cout << "Índice fuera de rango!\n";
                    }
                } else {
                    std::cout << "Entrada inválida!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_detalle << " ms, Memoria: " << memoria_detalle << " KB\n";
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }
                
            case 3: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                std::cout << "\nIngrese el ID a buscar: ";
                std::cin >> idBusqueda;
                
                monitor.iniciar_tiempo();
                
                if(const Persona* encontrada = buscarPorID(*personas, idBusqueda)) {
                    encontrada->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Buscar por ID", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 4: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                std::cout << "\nPresione 1 para buscar por país";
                std::cout << "\nPresione 2 para buscar por ciudad\n";

                int opcionBusqueda;
                std::cin >> opcionBusqueda;

                if (opcionBusqueda == 1) {
                    std::cout << "\nBuscando persona más longeva del país...";

                    monitor.iniciar_tiempo();
                    Persona encontrada = buscarMasLongevoPorValor(*personas);
                    encontrada.mostrar();
                    
                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más longeva por valor", tiempo_busqueda, memoria_busqueda);
                } else if (opcionBusqueda == 2) {
                    std::cout << "\nIngrese la ciudad: ";
                    std::string ciudad;
                    std::cin >> ciudad;

                    if (ciudad.empty() || !ciudadValida(ciudad)) {
                        std::cout << "Ciudad no puede estar vacía!\n";
                        break;
                    }

                    monitor.iniciar_tiempo();
                    Persona encontrada = buscarMasLongevoPorValorEnCiudad(*personas, ciudad);
                    encontrada.mostrar();
                    
                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más longeva por valor en ciudad", tiempo_busqueda, memoria_busqueda);
                } else {
                    std::cout << "Opción inválida!\n";
                }
                break;
            }

            case 5: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                std::cout << "\nPresione 1 para buscar por país";
                std::cout << "\nPresione 2 para buscar por ciudad\n";

                int opcionBusqueda;
                std::cin >> opcionBusqueda;

                if (opcionBusqueda == 1) {
                    std::cout << "\nBuscando persona más longeva por referencia...";
                    
                    monitor.iniciar_tiempo();
                    const Persona* encontrada = buscarMasLongevoPorReferencia(*personas);
                    encontrada->mostrar();
                    
                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar mas longeva por referencia", tiempo_busqueda, memoria_busqueda);
                } else if (opcionBusqueda == 2) {
                    std::cout << "\nIngrese la ciudad: ";
                    std::string ciudad;
                    std::cin >> ciudad;

                    if (ciudad.empty() || !ciudadValida(ciudad)) {
                        std::cout << "Ciudad no puede estar vacía!\n";
                        break;
                    }

                    monitor.iniciar_tiempo();
                    const Persona* encontrada = buscarMasLongevoPorReferenciaEnCiudad(*personas, ciudad);
                    encontrada->mostrar();
                    
                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más longeva por referencia en ciudad", tiempo_busqueda, memoria_busqueda);
                } else {
                    std::cout << "Opción inválida!\n";
                }
                break;
            }

            case 6: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                std::cout << "\nPresione 1 para buscar por país";
                std::cout << "\nPresione 2 para buscar por ciudad";
                std::cout << "\nPresione 3 para buscar por grupo de declaración (A, B o C)\n";

                int opcionBusqueda;
                std::cin >> opcionBusqueda;

                if (opcionBusqueda == 1) {
                    std::cout << "\nBuscando persona más rica por valor...";

                    monitor.iniciar_tiempo();
                    Persona encontrada = buscarMasPatrimonioPorValor(*personas);
                    encontrada.mostrar();
                    
                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar mas rica por valor", tiempo_busqueda, memoria_busqueda);
                } else if (opcionBusqueda == 2) {
                    std::cout << "\nIngrese la ciudad: ";
                    std::string ciudad;
                    std::cin >> ciudad;

                    if (ciudad.empty() || !ciudadValida(ciudad)) {
                        std::cout << "Ciudad no puede estar vacía!\n";
                        break;
                    }

                    monitor.iniciar_tiempo();
                    Persona encontrada = buscarMasPatrimonioPorValorEnCiudad(*personas, ciudad);
                    encontrada.mostrar();

                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más rica por valor en ciudad", tiempo_busqueda, memoria_busqueda);
                } else if (opcionBusqueda == 3) {
                    std::cout << "\nIngrese el grupo de declaración (A, B o C): ";
                    std::string grupo;
                    std::cin >> grupo;

                    if (grupo != "A" && grupo != "B" && grupo != "C") {
                        std::cout << "Grupo inválido. Debe ser A, B o C.\n";
                        break;
                    }

                    monitor.iniciar_tiempo();
                    Persona encontrada = buscarMasPatrimonioPorValorEnGrupo(*personas, grupo);
                    encontrada.mostrar();

                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más rica por valor en grupo", tiempo_busqueda, memoria_busqueda);
                } else {
                    std::cout << "Opción inválida!\n";
                }
                break;
            }

            case 7: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                std::cout << "\nPresione 1 para buscar por país";
                std::cout << "\nPresione 2 para buscar por ciudad";
                std::cout << "\nPresione 3 para buscar por grupo de declaración (A, B o C)\n";

                int opcionBusqueda;
                std::cin >> opcionBusqueda;

                if (opcionBusqueda == 1) {
                    std::cout << "\nBuscando persona más rica por referencia...";

                    monitor.iniciar_tiempo();
                    const Persona* encontrada = buscarMasPatrimonioPorReferencia(*personas);
                    encontrada->mostrar();
                    
                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar mas rica por referencia", tiempo_busqueda, memoria_busqueda);
                } else if (opcionBusqueda == 2) {
                    std::cout << "\nIngrese la ciudad: ";
                    std::string ciudad;
                    std::cin >> ciudad;

                    if (ciudad.empty() || !ciudadValida(ciudad)) {
                        std::cout << "Ciudad no puede estar vacía!\n";
                        break;
                    }

                    monitor.iniciar_tiempo();
                    const Persona* encontrada = buscarMasPatrimonioPorReferenciaEnCiudad(*personas, ciudad);
                    encontrada->mostrar();

                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más rica por referencia en ciudad", tiempo_busqueda, memoria_busqueda);
                } else if (opcionBusqueda == 3) {
                    std::cout << "\nIngrese el grupo de declaración (A, B o C): ";
                    std::string grupo;
                    std::cin >> grupo;

                    if (grupo != "A" && grupo != "B" && grupo != "C") {
                        std::cout << "Grupo inválido. Debe ser A, B o C.\n";
                        break;
                    }

                    monitor.iniciar_tiempo();
                    const Persona* encontrada = buscarMasPatrimonioPorReferenciaEnGrupo(*personas, grupo);
                    encontrada->mostrar();

                    double tiempo_busqueda = monitor.detener_tiempo();
                    long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                    std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                    monitor.registrar("Buscar persona más rica por referencia en grupo", tiempo_busqueda, memoria_busqueda);
                } else {
                    std::cout << "Opción inválida!\n";
                }
                break;
            }

            case 8: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                std::string grupo;
                std::cout << "\nIngrese el grupo a listar: ";
                std::cin >> grupo;

                monitor.iniciar_tiempo();
                
                auto personasGrupo = listarPersonasPorValorEnGrupo(*personas, grupo);
                std::cout << "\n\nPersonas en grupo " << grupo << " por valor: " << personasGrupo.size() << "\n";
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Listar por grupo por valor", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 9: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                
                std::string grupo;
                std::cout << "\nIngrese el grupo a listar: ";
                std::cin >> grupo;

                monitor.iniciar_tiempo();
                
                auto personasGrupo = listarPersonasPorReferenciaEnGrupo(*personas, grupo);
                std::cout << "\n\nPersonas en grupo " << grupo << " por referencia: " << personasGrupo.size() << "\n";
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Listar por grupo por referencia", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 10: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                monitor.iniciar_tiempo();
                verificarGruposMasivoPorValor(*personas);

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Verificar grupo por valor", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 11: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                monitor.iniciar_tiempo();
                verificarGruposMasivoPorReferencia(*personas);

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Verificar grupo por referencia", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 12: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                monitor.iniciar_tiempo();
                std::string grupoMayor = encontrarGrupoMayorPatrimonioPorValor(*personas);
                std::cout << "\nGrupo con mayor patrimonio en promedio por valor: " << grupoMayor << "\n";

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Encontrar grupo con mayor patrimonio (valor)", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 13: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                monitor.iniciar_tiempo();
                std::string grupoMayor = encontrarGrupoMayorPatrimonioPorReferencia(*personas);
                std::cout << "\nGrupo con mayor patrimonio en promedio por referencia: " << grupoMayor << "\n";

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Encontrar grupo con mayor patrimonio (referencia)", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 14: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                monitor.iniciar_tiempo();
                std::string grupoMayor = encontrarGrupoMayorLongevidadPorValor(*personas);
                std::cout << "\nGrupo con mayor longevidad en promedio por valor: " << grupoMayor << "\n";

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Encontrar grupo con mayor longevidad (valor)", tiempo_busqueda, memoria_busqueda);
                break;
            }

            case 15: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    std::cout << "Presione Enter para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }

                monitor.iniciar_tiempo();
                std::string grupoMayor = encontrarGrupoMayorLongevidadPorReferencia(*personas);
                std::cout << "\nGrupo con mayor longevidad en promedio por referencia: " << grupoMayor << "\n";

                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                std::cout << "Proceso terminado en " << tiempo_busqueda << " ms, Memoria: " << memoria_busqueda << " KB\n";
                monitor.registrar("Encontrar grupo con mayor longevidad (referencia)", tiempo_busqueda, memoria_busqueda);
                break;
            }
                
            case 16:
                monitor.mostrar_resumen();
                break;
                
            case 17:
                monitor.exportar_csv();
                break;
                
            case 18:
                std::cout << "Saliendo...\n";
                break;
                
            default:
                std::cout << "Opción inválida!\n";
        }
        
    } while(opcion != 18); // Updated exit condition to 18
    
    return 0;
}
