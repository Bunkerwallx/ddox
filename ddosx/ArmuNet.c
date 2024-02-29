#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#include <array>
#include <curl/curl.h>

// Constantes estáticas para los tamaños
static const size_t SALT_SIZE = 16;
static const size_t KEY_SIZE = 32;
static const size_t IV_SIZE = 16;

// URL del servidor remoto donde enviar el archivo cifrado
static const std::string SERVER_URL = "https://example.com/upload";

// Espacio de nombres para CryptoPP
using namespace CryptoPP;

// Clase para manejar los datos del archivo
class FileObject {
public:
    // Constructor que recibe los nombres de los archivos y genera la sal y el vector de inicialización
    FileObject(const std::string& in_filename, const std::string& out_filename) :
        filename(in_filename), out_filename(out_filename) {
        // Genera la sal aleatoria
        AutoSeededRandomPool rng;
        rng.GenerateIntoRandomized(salt);

        // Genera el vector de inicialización aleatorio
        rng.GenerateIntoRandomized(iv);
    }

    // Getters para los atributos
    const std::string& get_filename() const { return filename; }
    const std::string& get_out_filename() const { return out_filename; }
    const std::array<uint8_t, SALT_SIZE>& get_salt() const { return salt; }
    const std::array<uint8_t, IV_SIZE>& get_iv() const { return iv; }

private:
    std::string filename; // nombre del archivo
    std::string out_filename; // nombre del archivo de salida
    std::array<uint8_t, SALT_SIZE> salt; // sal para el cifrado
    std::array<uint8_t, IV_SIZE> iv; // vector de inicialización
};

// Función para cifrar el archivo
void aes_encrypt_file(const FileObject& file, const std::array<uint8_t, KEY_SIZE>& key) {
    // Abre el archivo de entrada
    std::ifstream in_file(file.get_filename(), std::ios::binary);
    if (!in_file) {
        // Lanza error si no puede abrir el archivo
        throw std::runtime_error("No se pudo abrir el archivo de entrada: " + file.get_filename());
    }

    // Abre el archivo de salida
    std::ofstream out_file(file.get_out_filename(), std::ios::binary);
    if (!out_file) {
        // Lanza error si no puede abrir el archivo
        throw std::runtime_error("No se pudo abrir el archivo de salida: " + file.get_out_filename());
    }

    try {
        // Crea el objeto de cifrado con la clave y el vector de inicialización
        CBC_Mode<AES>::Encryption enc(&key[0], KEY_SIZE, &file.get_iv()[0]);

        // Cifra el archivo y lo guarda en el archivo de salida
        FileSource(in_file, true, new StreamTransformationFilter(enc, new FileSink(out_file)));
    } catch (const CryptoPP::Exception &ex) {
        // Lanza error si ocurre una excepción
        throw std::runtime_error("Error al cifrar el archivo: " + std::string(ex.what()));
    }
}

// Función para generar la clave a partir de la contraseña y la sal
void generate_key(const std::string &password, const std::array<uint8_t, SALT_SIZE>& salt, std::array<uint8_t, KEY_SIZE>& key) {
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf2;
    pbkdf2.DeriveKeyFromSecret(password.data(), password.size(), &salt[0], SALT_SIZE, 10000, &key[0], KEY_SIZE);
}

// Función para enviar el archivo cifrado al servidor remoto
void send_file_to_server(const std::string& file_path) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, file_path.c_str(), CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error al enviar el archivo al servidor: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Archivo enviado con éxito" << std::endl;
        }

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }
}

// Función principal
int main() {
    try {
        // Pide al usuario la contraseña y los nombres de los archivos
        std::string password, in_filename, out_filename;
        std::cout << "Introduce la contraseña para el cifrado: ";
        std::cin >> password;
        std::cout << "Introduce el nombre del archivo a cifrar: ";
        std::cin >> in_filename;
        std::cout << "Introduce el nombre del archivo de salida: ";
        std::cin >> out_filename;

        // Crea el objeto de archivo con los nombres dados
        FileObject file(in_filename, out_filename);

        // Genera la clave a partir de la contraseña y la sal
        std::array<uint8_t, KEY_SIZE> key;
        generate_key(password, file.get_salt(), key);

        // Cifra el archivo con la clave
        aes_encrypt_file(file, key);

        // Envia el archivo cifrado al servidor
        send_file_to_server(file.get_out_filename());

    } catch (const std::exception &ex) {
        // Muestra el error si ocurre una excepción
        std::cerr << "Excepción: " << ex.what() << std::endl;
    }

    return 0;
}
