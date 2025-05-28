// operaciones.cpp
#define NOMINMAX

#include "operaciones.h"
#include <iostream>
#include <string>
#include "ConexionDB.h"
#include <limits>
#include <sstream> 

// --- Libros ---

void consultarLibros(MYSQL* con) {
    if (mysql_query(con, "SELECT id, titulo, autor, disponible, stock,idioma FROM libros")) {
        std::cerr << "Error: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "ID | Title  | Author | Available | Stock|Idiom\n";
    while (MYSQL_ROW row = mysql_fetch_row(res)) {
        std::cout
            << row[0] << " | "
            << row[1] << " | "
            << row[2] << " | "
            << row[3] << " | "
            << row[4] << " | "
            << row[5] << "\n";
    }
    mysql_free_result(res);
}

void consultarLibroPorId(MYSQL* con) {
    std::cout << "Enter book ID: ";
    int id;
    std::cin >> id;
    std::string q = "SELECT titulo, autor, disponible, stock,idioma FROM libros WHERE id=" + std::to_string(id);
    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    if (MYSQL_ROW row = mysql_fetch_row(res)) {
        std::cout << "Title: " << row[0]
            << "\nAuthor: " << row[1]
            << "\nBook Available: " << row[2]
            << "\nStock: " << row[3]
            << "\nIdiom: " << row[4] << "\n";
    }
    else {
        std::cout << "There is no book with ID=" << id << "\n";
    }
    mysql_free_result(res);
}


// --- Préstamos ---

void consultarPrestamosUsuario(MYSQL* con, const std::string& userCedula) {
    std::string q =
        "SELECT id, book_id, loan_date "
        "FROM loans "
        "WHERE user_id='" + userCedula +
        "' AND status='ongoing';";
    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error SELECT loans usuario: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "LoanID | BookID |Loan date\n";
    while (MYSQL_ROW r = mysql_fetch_row(res)) {
        std::cout
            << r[0] << "      | "
            << r[1] << "      | "
            << r[2] << "\n";
    }
    mysql_free_result(res);
}

void hacerPrestamo(MYSQL* con, const std::string& userCedula) {
    char opcion;

    // 1) Preguntar si quiere buscar por título
    std::cout << "Do you want to search for the book by title first? (s/n): ";
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (opcion == 's' || opcion == 'S') {
        buscarLibrosPorNombre(con);
    }

    // 2) Preguntar si desea realizar un préstamo
    std::cout << "Do you want to borrow a book? (y/n): ";
    std::cin >> opcion;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (opcion != 'y' && opcion != 'Y') {
        std::cout << "Loan cancelled, returning to menu.\n";
        return;
    }

    // 3) Ahora solicitamos el ID del libro
    int bookId;
    std::cout << "Enter the ID of the book to be borrowed: ";
    std::cin >> bookId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 4) Verificar disponibilidad y stock
    std::string q1 =
        "SELECT disponible, stock FROM libros WHERE id=" + std::to_string(bookId);
    if (mysql_query(con, q1.c_str())) {
        std::cerr << "Error SELECT books: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* r1 = mysql_store_result(con);
    if (!r1 || mysql_num_rows(r1) == 0) {
        std::cout << "Book does not exist.\n";
        if (r1) mysql_free_result(r1);
        return;
    }
    MYSQL_ROW row1 = mysql_fetch_row(r1);
    bool disponible = (row1[0] && std::string(row1[0]) == "1");
    int stock = row1[1] ? std::stoi(row1[1]) : 0;
    mysql_free_result(r1);

    if (!disponible || stock <= 0) {
        std::cout << "Book not available (stock=" << stock << ").\n";
        return;
    }

    // 5) Insertar el préstamo
    std::string q2 =
        "INSERT INTO loans(book_id, user_id) VALUES("
        + std::to_string(bookId) + ",'"
        + userCedula + "');";
    if (mysql_query(con, q2.c_str())) {
        std::cerr << "Error INSERT loan: " << mysql_error(con) << "\n";
        return;
    }
    unsigned long loanId = mysql_insert_id(con);
    std::cout << "Loan created. Loan ID=" << loanId << "\n";

    // 6) Actualizar stock y disponibilidad
    std::string q3 =
        "UPDATE libros SET "
        "stock = stock - 1, "
        "disponible = IF(stock - 1 > 0, 1, 0) "
        "WHERE id=" + std::to_string(bookId);
    if (mysql_query(con, q3.c_str())) {
        std::cerr << "Error UPDATE book: " << mysql_error(con) << "\n";
    }
}

void devolverPrestamo(MYSQL* con, const std::string& userCedula) {
    // Mostrar préstamos activos
    consultarPrestamosUsuario(con, userCedula);

    std::cout << "Enter the ID of the loan to be repaid: ";
    int loanId;
    std::cin >> loanId;

    // Verificar préstamo ongoing y obtener book_id
    std::string q1 =
        "SELECT book_id FROM loans "
        "WHERE id=" + std::to_string(loanId) +
        " AND status='ongoing'";
    if (mysql_query(con, q1.c_str())) {
        std::cerr << "Error SELECT loan: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* r1 = mysql_store_result(con);
    if (!r1 || mysql_num_rows(r1) == 0) {
        std::cout << "Invalid or already repaid loan.\n";
        mysql_free_result(r1);
        return;
    }
    int bookId = std::stoi(mysql_fetch_row(r1)[0]);
    mysql_free_result(r1);

    // Actualizar préstamo
    std::string q2 =
        "UPDATE loans "
        "SET return_date = CURDATE(), status = 'returned' "
        "WHERE id = " + std::to_string(loanId);
    if (mysql_query(con, q2.c_str())) {
        std::cerr << "Error UPDATE loan: " << mysql_error(con) << "\n";
        return;
    }

    // Incrementar stock y marcar disponible
    std::string q3 =
        "UPDATE libros "
        "SET stock = stock + 1, disponible = 1 "
        "WHERE id = " + std::to_string(bookId);
    if (mysql_query(con, q3.c_str())) {
        std::cerr << "Error UPDATE libro: " << mysql_error(con) << "\n";
        return;
    }

    std::cout << "Loan successfully repaid. Stock increased.\n";
}

// --- Para admin: ver todos los préstamos ---
void verTodosLosPrestamos(MYSQL* con) {
    std::string q =
        "SELECT l.id, b.titulo, u.username, l.loan_date, l.return_date, l.status "
        "FROM loans l "
        "JOIN libros b ON l.book_id = b.id "
        "JOIN users u  ON l.user_id = u.id "
        "ORDER BY l.loan_date DESC";
    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error SELECT loans: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "ID | Book | User | loan date  | Return date | loan status\n";
    while (MYSQL_ROW r = mysql_fetch_row(res)) {
        std::cout
            << r[0] << " | "
            << r[1] << " | "
            << r[2] << " | "
            << r[3] << " | "
            << (r[4] ? r[4] : std::string("NULL")) << " | "
            << r[5] << "\n";
    }
    mysql_free_result(res);
}




// —————————————————————————————
// 1) Mostrar todas las preguntas
// —————————————————————————————
void listarPreguntas(ConexionBD& cn) {
    MYSQL* con = cn.getConector();
    if (mysql_query(con, "SELECT id, pregunta FROM security_questions;")) {
        std::cerr << "Error loading questions: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "\nSecurity questions:\n";
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        std::cout << row[0] << ") " << row[1] << "\n";
    }
    mysql_free_result(res);
}

// —————————————————————————————
// 2) Registrar la respuesta del usuario
// —————————————————————————————
bool registrarPregunta(ConexionBD& cn, const std::string& cedula) {
    MYSQL* con = cn.getConector();

    // 1) Listar preguntas
    listarPreguntas(cn);

    // 2) Leer selección y respuesta
    int preguntaId;
    std::cout << "Select the question ID: ";
    std::cin >> preguntaId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string respuesta;
    std::cout << "Your answer: ";
    std::getline(std::cin, respuesta);

    // 3) Escapar y construir la consulta con ON DUPLICATE KEY
    char esc[512];
    mysql_real_escape_string(con, esc, respuesta.c_str(),
        static_cast<unsigned long>(respuesta.length()));

    std::string q =
        "INSERT INTO user_security(user_id, question_id, answer_hash) VALUES('"
        + cedula + "', "
        + std::to_string(preguntaId) + ", SHA2('" + esc + "',256)) "
        "ON DUPLICATE KEY UPDATE "
        " question_id = VALUES(question_id),"
        " answer_hash  = VALUES(answer_hash);";

    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error INSERT/UPDATE user_security: "
            << mysql_error(con) << "\n";
        return false;
    }

    std::cout << " Security Question "
        << (mysql_affected_rows(con) == 1 ? "registered" : "updated")
        << " correctly.\n";
    return true;
}


// —————————————————————————————
// 3) Validar la pregunta de seguridad
// —————————————————————————————
bool validarPregunta(ConexionBD& cn, const std::string& cedula) {
    MYSQL* con = cn.getConector();
    // obtenemos pregunta y hash
    std::string q =
        "SELECT sq.pregunta, us.answer_hash "
        "FROM user_security us "
        " JOIN security_questions sq ON us.question_id = sq.id "
        "WHERE us.user_id = '" + cedula + "';";
    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error SELECT user_security: " << mysql_error(con) << "\n";
        return false;
    }
    MYSQL_RES* res = mysql_store_result(con);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        std::cout << "You do not have a security question registered.\n";
        mysql_free_result(res);
        return false;
    }
    std::string pregunta = row[0];
    std::string hashGuardado = row[1];
    mysql_free_result(res);

    std::string resp;
    std::cout << "\nSecurity Question: " << pregunta << "\n";
    std::cout << "Answer: ";
    std::getline(std::cin, resp);

    // comparamos el hash
    std::string q2 =
        "SELECT 1 FROM user_security "
        "WHERE user_id='" + cedula +
        "' AND answer_hash = SHA2('" + resp + "',256);";
    if (mysql_query(con, q2.c_str())) {
        std::cerr << "Error validating response: " << mysql_error(con) << "\n";
        return false;
    }
    MYSQL_RES* res2 = mysql_store_result(con);
    bool ok = (res2 && mysql_num_rows(res2) == 1);
    mysql_free_result(res2);

    if (!ok) std::cout << "Incorrect answer.\n";
    return ok;
}

// —————————————————————————————
// 4) Registrar usuario (y su pregunta)
// —————————————————————————————
bool registrarUsuario(ConexionBD& cn) {
    MYSQL* con = cn.getConector();
    std::string cedula;
    std::cout << "ID: ";
    std::getline(std::cin, cedula);

    // verificar en estudiantes/profesores, asignar role, nombre, apellido...
    std::string nombre, apellido, role;
    MYSQL_RES* res;
    auto busca = [&](const char* tabla, const std::string& rolVal) {
        std::string qq =
            "SELECT nombre, apellido FROM " + std::string(tabla) +
            " WHERE cedula='" + cedula + "' LIMIT 1;";
        if (mysql_query(con, qq.c_str())) return false;
        res = mysql_store_result(con);
        if (MYSQL_ROW r = mysql_fetch_row(res)) {
            nombre = r[0];
            apellido = r[1];
            role = rolVal;
            mysql_free_result(res);
            return true;
        }
        mysql_free_result(res);
        return false;
        };

    if (!busca("estudiantes", "student") && !busca("profesores", "professor")) {
        std::cout << "ID not found.\n";
        return false;
    }

    // 3) Usuario y contraseña
    std::string user = nombre + "." + apellido;
    std::string pwd;
    std::cout << "Choose password: ";
    std::getline(std::cin, pwd);

    // 4) Insert en users
    std::string ins =
        "INSERT INTO users(id,username,password_hash,role) VALUES('"
        + cedula + "','" + user + "',SHA2('" + pwd + "',256),'"
        + role + "');";
    if (mysql_query(con, ins.c_str())) {
        std::cerr << "Error INSERT users: " << mysql_error(con) << "\n";
        return false;
    }

    // 5) Registrar pregunta de seguridad
    if (!registrarPregunta(cn, cedula)) return false;

    std::cout << " Registered " << user << " (" << role << ")\n";
    return true;
}

// —————————————————————————————
// 5) Reset password (validar pregunta)
// —————————————————————————————
bool resetPassword(ConexionBD& cn) {
    MYSQL* con = cn.getConector();
    std::string cedula;
    std::cout << "Forgot your password?\nID: ";
    std::getline(std::cin, cedula);

    // existe?
    std::string q = "SELECT 1 FROM users WHERE id='" + cedula + "';";
    if (mysql_query(con, q.c_str())) return false;
    MYSQL_RES* r = mysql_store_result(con);
    if (!r || mysql_num_rows(r) == 0) {
        mysql_free_result(r);
        std::cout << "No user exists.\n"; return false;
    }
    mysql_free_result(r);

    // validar pregunta
    if (!validarPregunta(cn, cedula)) return false;

    // pedir nueva clave
    std::string p1, p2;
    std::cout << "New password: "; std::getline(std::cin, p1);
    std::cout << "Confirm: ";       std::getline(std::cin, p2);
    if (p1 != p2) { std::cout << "They do not match.\n"; return false; }

    std::string upd =
        "UPDATE users SET password_hash=SHA2('" + p1 + "',256) "
        "WHERE id='" + cedula + "';";
    if (mysql_query(con, upd.c_str())) {
        std::cerr << "Error UPDATE pass: " << mysql_error(con) << "\n";
        return false;
    }
    std::cout << "Password updated\n";
    return true;
}

// 1) Ver la pregunta de seguridad actual
void verPreguntaSeguridad(ConexionBD& cn, const std::string& userId) {
    MYSQL* con = cn.getConector();
    std::string q =
        "SELECT sq.id, sq.pregunta "
        "FROM user_security us "
        " JOIN security_questions sq ON us.question_id = sq.id "
        "WHERE us.user_id = '" + userId + "';";
    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error SELECT pregunta actual: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    if (MYSQL_ROW row = mysql_fetch_row(res)) {
        std::cout << "Your current question (" << row[0] << "): " << row[1] << "\n";
    }
    else {
        std::cout << "You do not have a security question registered.\n";
    }
    mysql_free_result(res);
}


void listarCategorias(MYSQL* con) {
    if (mysql_query(con, "SELECT id, nombre FROM categorias;")) {
        std::cerr << "Error loading categories: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "\nID |Category\n";
    while (MYSQL_ROW row = mysql_fetch_row(res)) {
        std::cout << row[0] << " | " << row[1] << "\n";
    }
    mysql_free_result(res);
}

void asignarCategorias(MYSQL* con, unsigned long libroId) {
    listarCategorias(con);

    std::cout << "Enter category IDs separated by commas (ej: 1,3): ";
    // No necesitamos cin.ignore() aquí si justo antes leíste con getline; 
    // aseguramos que esté limpio:
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    std::getline(std::cin, line);

    std::stringstream ss(line);
    int catId;
    while (ss >> catId) {
        std::string q =
            "INSERT INTO libro_categoria(libro_id, categoria_id) VALUES("
            + std::to_string(libroId) + ","
            + std::to_string(catId) + ");";
        if (mysql_query(con, q.c_str())) {
            std::cerr << "Error assigning category" << catId
                << ": " << mysql_error(con) << "\n";
        }
        else {
            std::cout << "Category " << catId << " added successfully.\n";
        }
        if (ss.peek() == ',') ss.ignore();
    }
}

void agregarLibro(MYSQL* con) {
    // 1) Leemos datos básicos
    std::string titulo, autor;
    int disponible, stock;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Title: ";     std::getline(std::cin, titulo);
    std::cout << "Author: ";      std::getline(std::cin, autor);
    std::cout << "Available (0/1): "; std::cin >> disponible;
    std::cout << "Stock: ";      std::cin >> stock;

    // 2) Escapamos para evitar SQL injection
    char eTitulo[512], eAutor[512];
    mysql_real_escape_string(con, eTitulo, titulo.c_str(),
        static_cast<unsigned long>(titulo.length()));
    mysql_real_escape_string(con, eAutor, autor.c_str(),
        static_cast<unsigned long>(autor.length()));

    // 3) Insertamos en la tabla libros
    std::string q =
        "INSERT INTO libros(titulo, autor, disponible, stock) VALUES('"
        + std::string(eTitulo) + "','"
        + std::string(eAutor) + "',"
        + std::to_string(disponible) + ","
        + std::to_string(stock) + ");";

    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error INSERT libro: " << mysql_error(con) << "\n";
        return;
    }

    // 4) Recuperamos el ID del libro recién creado
    unsigned long libroId = mysql_insert_id(con);
    std::cout << " Book added. ID=" << libroId << "\n";

    // 5) Asignar categorías al nuevo libro
    asignarCategorias(con, libroId);
}
void filtrarPorCategoria(MYSQL* con) {
    // 1) Mostrar categorías disponibles
    if (mysql_query(con, "SELECT id, nombre FROM categorias;")) {
        std::cerr << "Error cargando categorías: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "\nID |Category\n";
    while (MYSQL_ROW row = mysql_fetch_row(res)) {
        std::cout << row[0] << " | " << row[1] << "\n";
    }
    mysql_free_result(res);

    // 2) Leer selección
    std::cout << "Select Category ID to filter: ";
    int catId;
    std::cin >> catId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 3) Ejecutar consulta filtrada
    std::string q =
        "SELECT l.id, l.titulo, l.autor, l.disponible, l.stock "
        "FROM libros l "
        "JOIN libro_categoria lc ON l.id = lc.libro_id "
        "WHERE lc.categoria_id = " + std::to_string(catId) + ";";

    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error filtering books: " << mysql_error(con) << "\n";
        return;
    }

    // 4) Mostrar resultados
    MYSQL_RES* res2 = mysql_store_result(con);
    std::cout << "\nID | Title | author | book available | Stock\n";
    while (MYSQL_ROW r = mysql_fetch_row(res2)) {
        std::cout
            << r[0] << " | "
            << r[1] << " | "
            << r[2] << " | "
            << r[3] << " | "
            << r[4] << "\n";
    }
    mysql_free_result(res2);
}

// --- 1) Seleccionar idioma ---
std::string seleccionarIdioma(MYSQL* con) {
    std::cout << "\nSelect language:\n"
        << "1) spanish\n"
        << "2) English\n> ";
    int opc; std::cin >> opc;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return (opc == 2 ? "english" : "spanish");
}

// --- 2) Seleccionar categoría (reusa listarCategorias) ---
int seleccionarCategoria(MYSQL* con) {
    listarCategorias(con);
    std::cout << "Enter Category ID: ";
    int cat; std::cin >> cat;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return cat;
}

// --- 3) Seleccionar orden ---
std::string seleccionarOrden() {
    std::cout << "\nSort by title:\n"
        << "1) Upward\n"
        << "2) Falling\n> ";
    int opc; std::cin >> opc;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return (opc == 2 ? "DESC" : "ASC");
}


void filtrarLibros(MYSQL* con) {
    // 1) Elegir idioma
    std::string idioma = seleccionarIdioma(con);

    // 2) Elegir categoría
    int catId = seleccionarCategoria(con);

    // --- Consulta base sin ORDER BY ---
    std::ostringstream baseQ;
    baseQ
        << "SELECT l.id, l.titulo, l.autor, l.disponible, l.stock, l.idioma "
        << "FROM libros l "
        << "JOIN libro_categoria lc ON l.id = lc.libro_id "
        << "WHERE l.idioma = '" << idioma << "' "
        << "AND lc.categoria_id = " << catId << ";";
    std::string q0 = baseQ.str();

    // 3) Ejecutar y mostrar resultados sin orden
    if (mysql_query(con, q0.c_str())) {
        std::cerr << "Error filtering books: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res0 = mysql_store_result(con);
    std::cout << "\nUNSORTED RESULT:\n"
        << "ID | Title | author | book available | Stock | language\n";
    while (MYSQL_ROW r = mysql_fetch_row(res0)) {
        std::cout
            << r[0] << " | "
            << r[1] << " | "
            << r[2] << " | "
            << r[3] << " | "
            << r[4] << " | "
            << r[5] << "\n";
    }
    mysql_free_result(res0);

    // 4) Preguntar orden
    std::cout << "\nDo you want to sort the results?\n"
        << "1) Yes, ascending\n"
        << "2) Yes, descending\n"
        << "0) Do not return to the menu\n> ";
    int ordOpc; std::cin >> ordOpc;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (ordOpc == 1 || ordOpc == 2) {
        std::string orden = (ordOpc == 1 ? "ASC" : "DESC");
        // reconstruir la query con ORDER BY
        std::ostringstream q1;
        q1
            << "SELECT l.id, l.titulo, l.autor, l.disponible, l.stock, l.idioma "
            << "FROM libros l "
            << "JOIN libro_categoria lc ON l.id = lc.libro_id "
            << "WHERE l.idioma = '" << idioma << "' "
            << "AND lc.categoria_id = " << catId << " "
            << "ORDER BY l.titulo " << orden << ";";
        if (mysql_query(con, q1.str().c_str())) {
            std::cerr << "Error ordenando libros: " << mysql_error(con) << "\n";
            return;
        }
        MYSQL_RES* res1 = mysql_store_result(con);
        std::cout << "\nRESULTADO ORDENADO (" << orden << "):\n"
            << "ID | Title| Author | book available | Stock |language \n";
        while (MYSQL_ROW r = mysql_fetch_row(res1)) {
            std::cout
                << r[0] << " | "
                << r[1] << " | "
                << r[2] << " | "
                << r[3] << " | "
                << r[4] << " | "
                << r[5] << "\n";
        }
        mysql_free_result(res1);
    }

    // 5) Sub-menú de búsqueda por ID
    std::cout << "\nOpciones:\n"
        << "1)Return to the main menu\n"
        << "2) Search for a book by ID within this filter\n> ";
    int subOpc; std::cin >> subOpc;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (subOpc == 2) {
        std::cout << "Enter book ID: ";
        int id; std::cin >> id;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // podemos usar la misma condición idioma+categoría
        std::ostringstream q2;
        q2
            << "SELECT l.id, l.titulo, l.autor, l.disponible, l.stock, l.idioma "
            << "FROM libros l "
            << "JOIN libro_categoria lc ON l.id = lc.libro_id "
            << "WHERE l.id = " << id
            << " AND l.idioma = '" << idioma << "'"
            << " AND lc.categoria_id = " << catId << ";";
        if (mysql_query(con, q2.str().c_str())) {
            std::cerr << "Error SELECT por ID: " << mysql_error(con) << "\n";
            return;
        }
        MYSQL_RES* res2 = mysql_store_result(con);
        if (MYSQL_ROW rw = mysql_fetch_row(res2)) {
            std::cout << "\nID: " << rw[0]
                << "\nTitle: " << rw[1]
                << "\nAutor: " << rw[2]
                << "\nBook available: " << rw[3]
                << "\nStock: " << rw[4]
                << "\nIdiom: " << rw[5] << "\n";
        }
        else {
            std::cout << "There is no book with that ID in this filter..\n";
        }
        mysql_free_result(res2);
    }
}


//temporal
void listarPorIdioma(MYSQL* con) {
    if (mysql_query(con,
        "SELECT id, titulo, autor, disponible, stock, idioma "
        "FROM libros WHERE idioma='spanish';"))
    {
        std::cerr << "Error SELECT idioma: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "\n--- Books in Spanish ---\n";
    while (MYSQL_ROW r = mysql_fetch_row(res)) {
        std::cout << r[0] << " | " << r[1] << " | " << r[5] << "\n";
    }
    mysql_free_result(res);
}

void listarLibrosPorCategoria(MYSQL* con) {
    if (mysql_query(con,
      "SELECT l.id, l.titulo, c.nombre "
      "FROM libros l "
      "JOIN libro_categoria lc ON l.id = lc.libro_id "
      "JOIN categorias c   ON lc.categoria_id = c.id;"))
    {
        std::cerr << "Error JOIN libro_categoria: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    std::cout << "\n--- All Book–Category relationships ---\n";
    while (MYSQL_ROW r = mysql_fetch_row(res)) {
        std::cout << r[0] << " | " << r[1] << " | " << r[2] << "\n";
    }
    mysql_free_result(res);
}




void buscarLibrosPorNombre(MYSQL* con) {
    std::cout << "\nEnter the title to search for: ";
    // No hagas cin.ignore() aquí si llamas siempre desde un menú
    std::string fragmento;
    std::getline(std::cin, fragmento);

    if (fragmento.empty()) {
        std::cout << "Nothing to look for.\n";
        return;
    }

    // Escapar la entrada
    char esc[1024];
    unsigned long len = mysql_real_escape_string(
        con,
        esc,
        fragmento.c_str(),
        static_cast<unsigned long>(fragmento.length())
    );
    // Asegúrate de terminar el string C
    if (len < sizeof(esc)) esc[len] = '\0';

    // Construye el patrón "%fragmento%"
    std::string likePattern = "%" + std::string(esc, len) + "%";

    // Ejecuta el SELECT con LIKE
    std::string q =
        "SELECT id, titulo, autor, disponible, stock, idioma "
        "FROM libros "
        "WHERE titulo LIKE '" + likePattern + "';";

    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error SELECT LIKE: " << mysql_error(con) << "\n";
        return;
    }

    MYSQL_RES* res = mysql_store_result(con);
    if (!res) {
        std::cout << "No results found.\n";
        return;
    }

    std::cout << "\nID | Title | Author | book available? | Stock |language\n";
    bool any = false;
    while (MYSQL_ROW r = mysql_fetch_row(res)) {
        any = true;
        std::cout
            << r[0] << " | "
            << r[1] << " | "
            << r[2] << " | "
            << r[3] << " | "
            << r[4] << " | "
            << r[5] << "\n";
    }
    mysql_free_result(res);

    if (!any) {
        std::cout << "No matching books were found.\n";
    }
}

//hola 
void buscarEstudianteYPrestamos(MYSQL* con) {
    std::string fragment;
    std::cout << "Enter student first or last name to search: ";
    std::getline(std::cin, fragment);

    // 1) Buscar estudiantes por nombre o apellido
    std::string q =
        "SELECT cedula, nombre, apellido "
        "FROM estudiantes "
        "WHERE nombre LIKE '%" + fragment + "%' "
        "   OR apellido  LIKE '%" + fragment + "%';";

    if (mysql_query(con, q.c_str())) {
        std::cerr << "Error searching students: " << mysql_error(con) << "\n";
        return;
    }
    MYSQL_RES* res = mysql_store_result(con);
    if (!res || mysql_num_rows(res) == 0) {
        std::cout << "No students found matching \"" << fragment << "\".\n";
        if (res) mysql_free_result(res);
        return;
    }

    std::cout << "ID | First Name | Last Name\n";
    while (MYSQL_ROW row = mysql_fetch_row(res)) {
        std::cout
            << row[0] << " | "
            << row[1] << " | "
            << row[2] << "\n";
    }
    mysql_free_result(res);

    // 2) Pedir al profesor que elija un ID
    std::cout << "Enter the student ID to view active loans (or 0 to cancel): ";
    int studentId;
    std::cin >> studentId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (studentId == 0) {
        std::cout << "Operation cancelled.\n";
        return;
    }

    // 3) Reutilizar consultarPrestamosUsuario para mostrar los préstamos
    std::cout << "\nActive loans for student " << studentId << ":\n";
    consultarPrestamosUsuario(con, std::to_string(studentId));
}