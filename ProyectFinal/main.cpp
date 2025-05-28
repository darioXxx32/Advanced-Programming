#define NOMINMAX 
#include "Timer.h"
#include <iostream>
#include <string>
#include <limits>

#include "ConexionDB.h"
#include "operaciones.h"

static void pause() {
    std::cout << "\nPress ENTER to continue..." << std::flush;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// “Books” menu for student/professor
static void menuBooks(MYSQL* con, const std::string& userId) {
    int choice;
    Timer T;
    do {
        std::cout << "\n--- Books ---\n"
                  << "1) View all books\n"
                  << "2) Get book by ID\n"
                  << "3) Filter books (language + category)\n"
                  << "4) Search by title\n"
                  << "0) Back\n> ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1:
                for (int i = 0; i < 1; ++i) {
                    T.run("List all books", consultarLibros, con);
                }

                std::cout << "\nAverage time over 1 runs: "
                    << T.average() << " ms\n";
                pause();
                break;
            case 2:
                consultarLibroPorId(con);
                pause();
                break;
            case 3:
                filtrarLibros(con);
                pause();
                break;
            case 4:
                buscarLibrosPorNombre(con);
                pause();
                break;
            case 0:
                break;
            default:
                std::cout << "Invalid option.\n";
                pause();
        }
    } while (choice != 0);
}

// “Loans” menu for student
static void menuLoans(MYSQL* con, const std::string& userId) {
    int choice;
    Timer listTimer, loanTimer;
    do {
        std::cout << "\n--- Loans ---\n"
                  << "1) Issue loan\n"
                  << "2) Return loan\n"
                  << "3) My active loans\n"
                  << "0) Back\n> ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1:
                for (int i = 0;i < 1;i++)
                    loanTimer.run("Issue loan", hacerPrestamo, con, userId);

                std::cout << "Avg listing: " << listTimer.average() << " ms\n"
                    << "Avg loan:    " << loanTimer.average() << " ms\n";
                pause();
                break;
            case 2:
                devolverPrestamo(con, userId);
                pause();
                break;
            case 3:
                consultarPrestamosUsuario(con, userId);
                pause();
                break;
            case 0:
                break;
            default:
                std::cout << "Invalid option.\n";
                pause();
        }
    } while (choice != 0);
}

// “Security” menu for student/professor
static void menuSecurity(ConexionBD& cn, const std::string& userId) {
    int choice;
    do {
        std::cout << "\n--- Security ---\n"
                  << "1) View current security question\n"
                  << "2) Change security question\n"
                  << "3) Change password\n"
                  << "0) Back\n> ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1:
                verPreguntaSeguridad(cn, userId);
                pause();
                break;
            case 2:
                registrarPregunta(cn, userId);  // insert or update
                pause();
                break;
            case 3:
                resetPassword(cn);
                pause();
                break;
            case 0:
                break;
            default:
                std::cout << "Invalid option.\n";
                pause();
        }
    } while (choice != 0);
}

int main() {
    
    //  ===== Welcome =====
    std::cout << "===============================\n"
              << "  Welcome to BibliotecaTech  \n"
              << "===============================\n\n";

    ConexionBD cn;
    if (!cn.abrir_conexion()) {
        std::cerr << "Could not connect to the database\n";
        return 1;
    }

    //  ===== Initial Menu =====
    while (true) {
        std::cout << "\n--- Main Menu ---\n"
                  << "1) Register\n"
                  << "2) Log in\n"
                  << "3) Forgot my password\n"
                  << "0) Exit\n> ";
        int initChoice;
        std::cin >> initChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (initChoice == 0) {
            cn.cerrar_conexion();
            return 0;
        }
        else if (initChoice == 1) {
            if (registrarUsuario(cn)) {
                std::cout << "Registration successful. You can now log in.\n";
            }
            pause();
        }
        else if (initChoice == 2) {
            break;
        }
        else if (initChoice == 3) {
            resetPassword(cn);
            pause();
        }
        else {
            std::cout << "Invalid option.\n";
            pause();
        }
    }

    //  ===== Log in =====
    std::string username, password, role, userId;
    std::cout << "\nUsername: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    if (!cn.login(username, password, userId, role)) {
        std::cout << "Login failed\n";
        return 1;
    }
    std::cout << "Login OK. User ID: " << userId
              << ", Role: " << role << "\n";

    MYSQL* con = cn.getConector();

    //  ===== Role‐based Menu =====
    if (role == "student" || role == "professor") {
        int choice;
        do {
            std::cout << "\n--- "
                << (role == "student" ? "Student" : "Professor")
                << " Menu ---\n"
                << "1) Books\n";
            if (role == "student") {
                std::cout << "2) Loans\n"
                    << "3) Security\n"
                    << "0) Exit\n"
                    << "> ";
            }
            else {
                std::cout << "2) Loans\n"
                    << "3) Search Student Loans\n"
                    << "4) Security\n"
                    << "0) Exit\n"
                    << "> ";
            }

            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 0) break;

            if (role == "student") {
                switch (choice) {
                case 1: menuBooks(con, userId);   break;
                case 2: menuLoans(con, userId);   break;
                case 3: menuSecurity(cn, userId); break;
                default:
                    std::cout << "Invalid option.\n"; pause();
                }
            }
            else { // professor
                switch (choice) {
                case 1:
                    menuBooks(con, userId);
                    break;
                case 2:
                    menuLoans(con, userId);
                    break;
                case 3:
                    buscarEstudianteYPrestamos(con);
                    break;                // <-- add this break
                case 4:
                    menuSecurity(cn, userId);
                    break;
                default:
                    std::cout << "Invalid option.\n"; pause();
                }
            }
        } while (true);
    }

    else if (role == "admin") {
        int choice;
        do {
            std::cout << "\n--- Admin Menu ---\n"
                      << "1) View all books\n"
                      << "2) Add book\n"
                      << "3) View all loans\n"
                      << "0) Exit\n> ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            switch (choice) {
                case 1:
                    consultarLibros(con);
                    pause();
                    break;
                case 2:
                    agregarLibro(con);
                    pause();
                    break;
                case 3:
                    verTodosLosPrestamos(con);
                    pause();
                    break;
                case 0:
                    break;
                default:
                    std::cout << "Invalid option.\n";
                    pause();
            }
        } while (choice != 0);
    }
    else {
        std::cout << "Unknown role, exiting.\n";
    }

    cn.cerrar_conexion();
    return 0;
}
