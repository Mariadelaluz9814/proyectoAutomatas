#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

vector<string> lineaArchivo;
vector<string> tokensTerminados;
vector<string> tokensError;

int estados[20][33] = {
        //      l       L        d          _        .        E       e         +        -       =         >         <         |        &          !        '        "         #         /        *        %         [         ]        (         )        {          }       ,          ;      /t        /n       /b         dif
        {1,   2,   3,   506, 506, 2,   1,   105, 106, 9,   11,  10,  14,  13,  12,  15,  17,  19,  108, 107, 128, 121, 122, 119, 120, 129, 130, 124, 123, 0,   0,   0,   506},
        {1,   2,   2,   2,   100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
        {2,   2,   2,   2,   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101},
        {102, 102, 3,   102, 4,   102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102},
        {500, 500, 5,   500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
        {103, 103, 5,   103, 103, 6,   6,   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103},
        {501, 501, 8,   501, 501, 501, 501, 7,   7,   501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501},
        {502, 502, 8,   502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502},
        {104, 104, 8,   104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104},
        {109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109},
        {111, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111},
        {113, 113, 113, 113, 113, 113, 113, 113, 113, 114, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113},
        {116, 116, 116, 116, 116, 116, 116, 116, 116, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116},
        {503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 117, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503},
        {504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 118, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504},
        {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  505, 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
        {507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 125, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507},
        {17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  18,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17},
        {126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 17,  126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126},
        {19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  127, 19,  19}
};
vector<string> palabrasReservadas = {"class",
                                     "endclass",
                                     "int",
                                     "float",
                                     "char",
                                     "string",
                                     "bool",
                                     "if",
                                     "else",
                                     "do",
                                     "while",
                                     "input",
                                     "output",
                                     "def",
                                     "to",
                                     "break",
                                     "loop"};

vector<string> leerArchivo() {
    string archivoLeer = "../codigotext.txt";
    ifstream archivo(archivoLeer);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo " << archivoLeer << endl;
    }
    string linea;

    while (getline(archivo, linea)) {
        lineaArchivo.push_back(linea);
    }
    archivo.close();
    return lineaArchivo;
}

string quitarEspacios(string basicString);

int comparaciones(char caracter, int estado) {
    if (caracter >= 'a' && caracter <= 'z') {
        if (estado == 5 && caracter == 'e') {
            return 6;
        }
        return 0;

    } else if (caracter >= 'A' && caracter <= 'Z') {
        if (estado == 5 && caracter == 'E') {
            return 6;
        }
        return 1;

    } else if (caracter >= '0' && caracter <= '9') {
        return 2;
    }
    switch (caracter) {
        case '_':
            return 3;
        case '.':
            return 4;
        case '+':
            return 7;
        case '-':
            return 8;
        case '=':
            return 9;
        case '>':
            return 10;
        case '<':
            return 11;
        case '|':
            return 12;
        case '&':
            return 13;
        case '!':
            return 14;
        case '\'':
            return 15;
        case '\"':
            return 16;
        case '#':
            return 17;
        case '/':
            return 18;
        case '*':
            return 19;
        case '%':
            return 20;
        case '[':
            return 21;
        case ']':
            return 22;
        case '(':
            return 23;
        case ')':
            return 24;
        case '{':
            return 25;
        case '}':
            return 26;
        case ',':
            return 27;
        case ';':
            return 28;
        case '\t':
            return 29;
        case '\n':
            return 30;
        case '\b':
            return 31;
        default:
            return 32;
    }
}/**/

int compararPalabrasReservadas(string valor, int valorEstado) {
    if (valorEstado > 101) {
        return valorEstado;
    }
    for (const auto &palabra: palabrasReservadas) {
        bool siIgual = valor == palabra;
        if (valor == palabra) {
            return 100;
        }
    }
    return 101;
}

string finalizar(int valor) {
    switch (valor) {
        case 100: //metodo comaración para palabra reservada.
            return "Terminacion 100: es una PALABRA RESERVADA.";
        case 101: //metodo comaración para identificador.
            return "Terminacion 101: es un IDENTIFICADOR.";
        case 102:
            return "Terminacion 102: numero ENTERO.";
        case 103:
            return "Terminacion 103: numero REAL.";
        case 104:
            return "Terminacion 104: numero de NOTACION CIENTIFICA.";
        case 105:
            return "Terminacion 105: es una SUMA.";
        case 106:
            return "Terminacion 106: es una RESTA.";
        case 107:
            return "Terminacion 107: es una MULTIPLICACION.";
        case 108:
            return "Terminacion 108: es una DIVISION.";
        case 109:
            return "Terminacion 109: es una ASIGNACION.";
        case 110:
            return "Terminacion 110: IGUAL.";
        case 111:
            return "Terminacion 111: MENOR.";
        case 112:
            return "Terminacion 112: MENOR IGUAL.";
        case 113:
            return "Terminacion 113: MAYOR.";
        case 114:
            return "Terminacion 114: MAYOR IGUAL.";
        case 115:
            return "Terminacion 115: DIFERENTE.";
        case 116:
            return "Terminacion 116: NOT.";
        case 117:
            return "Terminacion 117: AND.";
        case 118:
            return "Terminacion 118: OR.";
        case 119:
            return "Terminacion 119: PARENTESIS QUE ABRE.";
        case 120:
            return "Terminacion 120: PARENTESIS QUE CIERRA.";
        case 121:
            return "Terminacion 121: CORCHETE QUE ABRE.";
        case 122:
            return "Terminacion 122: CORCHETE QUE CIERRA.";
        case 123:
            return "Terminacion 123: PUNTO Y COMA.";
        case 124:
            return "Terminacion 124: es una COMA.";
        case 125:
            return "Terminacion 125: CARACTER.";
        case 126:
            return "Terminacion 126: STRING.";
        case 127:
            return "Terminacion 127: COMENTARIO DE LINEA.";
        case 128:
            return "Terminacion 128: MODULUS.";
        case 129:
            return "Terminacion 129: es una LLAVE QUE ABRE.";
        case 130:
            return "Terminacion 130: es una LLAVE QUE CIERRA.";
        default:
            return "Terminación no registrada.";
    }

}

string errores(int valor) {
    switch (valor) {
        case 500:
            return "ERROR 500: Es un valor flotante, se espera un DIGITO despues del PUNTO.";
        case 501:
            return "ERROR 501: Es un numero de NOTACION CIENTIFICA, se espera un +, - o DIGITO.";
        case 502:
            return "ERROR 502: ES un numero de NOTACION CIENTIFICA, se espera un DIGITO.";
        case 503:
            return "ERROR 503: Es un operador AND, se espera un &.";
        case 504:
            return "ERROR 504: Es un operador OR, se espera un |.";
        case 505:
            return "ERROR 505: Es un caracter, se espera un CARACTER.";
        case 506:
            return "ERROR 506: Es un delimitador usado incorrectamente.";
        case 507:
            return "ERROR 507: ES un caracter, se espera una ' .";
        default:
            return "ERROR no registrado";
    }
}

void analisis() {
    vector<string> lineasarchivo = leerArchivo();
    int estado;
    int columna = 0;
    int numeroLinea = 0;
    int caracterActual;
    int posicionCadena = 0;
    string token;

    while (numeroLinea < lineaArchivo.size()) {
        posicionCadena = 0;
        int longitudLinea = lineasarchivo[numeroLinea].size();
        estado = 0;
        token = "";

        while (posicionCadena < longitudLinea) {
            caracterActual = lineasarchivo[numeroLinea].at(posicionCadena);
            if (caracterActual == '#') {
                if (lineasarchivo[numeroLinea].at(0) == '#') {
                    tokensTerminados.push_back(lineasarchivo[numeroLinea] + " ->" + finalizar(127));
                } else {
                    tokensTerminados.push_back(
                            lineasarchivo[numeroLinea].substr(caracterActual - 1) + " ->" + finalizar(127));
                }
            }

            columna = comparaciones(caracterActual, estado);
            estado = estados[estado][columna];
            token += caracterActual;

            if (estado > 19) {
                if (token == " ") {
                    token = "";
                    estado = 0;
                }

                token = quitarEspacios(token);

                if (estado >= 100 && estado <= 130) {
                    //Imprimir finalizacion de tokens
                    if (estado == 100 || estado == 101) {
                        estado = compararPalabrasReservadas(token, estado);
                    }
                    tokensTerminados.push_back(token + " -> " + finalizar(estado));
                    token = "";
                    estado = 0;
                } else if (estado >= 500 && estado <= 507) {
                    //Imprimir errores de tokens
                    tokensError.push_back(token + " -> " + errores(estado));
                    token = "";
                    estado = 0;
                }
            }
            posicionCadena++;
        }
            numeroLinea++;
    }
    cout << "Lineas de archivo: " << endl;
    for (int i = 0; i < lineasarchivo.size(); ++i) {
        cout << lineasarchivo[i] << endl;
    }
    cout << "\nTokens terminados: " << endl;
    for (int i = 0; i < tokensTerminados.size(); ++i) {
        cout << tokensTerminados[i] << endl;
    }
    cout << "Tokens con errores: " << endl;
    for (int i = 0; i < tokensError.size(); ++i) {
        cout << tokensError[i] << endl;
    }
}

string quitarEspacios(string basicString) {
    string sin_espacios = "";

    for (int i = 0; i < basicString.length(); i++) {
        if (basicString[i] != ' ') {
            sin_espacios += basicString[i];
        }
    }

    return sin_espacios;
}

int main() {
    analisis();
    return 0;
}
