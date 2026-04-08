#include "client.h"
#include <string.h>


int main(void){
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Hola! Soy un log");

	/* ---------------- CONFIG ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);
	log_info(logger, "CLAVE: %s", valor);

	/* ---------------- CONEXION ---------------- */

	conexion = crear_conexion(ip, puerto);

	/* ---------------- ENVIO ---------------- */

	// Enviamos la clave
	enviar_mensaje(valor, conexion);

	// Enviamos las líneas como paquete
	paquete(conexion);

	/* ---------------- FIN ---------------- */

	terminar_programa(conexion, logger, config);

	return 0;
}

/* ================= FUNCIONES ================= */

t_log* iniciar_logger(void){
	return log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);
}

t_config* iniciar_config(void){
	t_config* config = config_create("cliente.config");

	if (config == NULL) {
		printf("Error: no se pudo leer el archivo de config\n");
		abort();
	}

	return config;
}

void paquete(int conexion){
	char* leido;
	t_paquete* paquete = crear_paquete();

	leido = readline("> ");

	while(leido != NULL && strcmp(leido, "") != 0)
	{
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline("> ");
	}

	free(leido);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}