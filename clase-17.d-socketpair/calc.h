enum data_type { INTEGER, DOUBLE };
enum data_op { SUMAR, RESTAR, MULTIPLICAR, DIVIDIR };
struct data_packet {
	int id;
	enum data_type type;
	enum data_op   op;
	union {
		double d[3];
		int    i[3];
	}u;
};

int calc_server(int fd);
int calc_client_integer(int fd, enum data_op operation, int a, int b);
