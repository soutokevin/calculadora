#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum type {
  Other,
  Number,
  Add,
  Subtract,
  Multiply,
  Divide,
  OpenParen,
  CloseParen
} type_t;

char* type_name(type_t type) {
  switch (type) {
    case Number:
      return "Number";
    case Add:
      return "Add";
    case Subtract:
      return "Subtract";
    case Multiply:
      return "Multiply";
    case Divide:
      return "Divide";
    case OpenParen:
      return "OpenParen";
    case CloseParen:
      return "CloseParen";
    default:
      return "Invalid";

  }
}

// ---------------------------------------------------------------------------------------------- //
//                                 Definição do nó da lista ligada                                //
// ---------------------------------------------------------------------------------------------- //

typedef struct node {
  type_t type;
  double data;
  struct node *next;
} node_t;

node_t* new_node(type_t type, double data, node_t* next) {
  node_t *node = (node_t*)malloc(sizeof(node_t));

  node -> type = type;
  node -> data = data;
  node -> next = next;

  return node;
}

void print_node(node_t* node) {
  printf("{ type: %s, data: %f }\n", type_name(node -> type), node -> data);
}
// Fim do nó ------------------------------------------------------------------------------------ //

// ---------------------------------------------------------------------------------------------- //
//                                    Definição da lista ligada                                   //
// ---------------------------------------------------------------------------------------------- //
typedef struct list {
  node_t *start, *end;
} list_t;

list_t* new_list() {
  list_t *list = (list_t *)malloc(sizeof(list_t));
  list -> start = NULL;
  list -> end = NULL;
  return list;
}

void drop_list(list_t *list) {
  node_t *current = list -> start;
  while (current) {
    node_t *next = current -> next;
    free(current);
    current = next;
  }
  free(list);
}

void print_list(list_t *list) {
  node_t *current = list -> start;
  while (current) {
    print_node(current);
    current = current -> next;
  }
}

bool is_empty_list(list_t *list) {
  return list -> start == NULL;
}

void push_start(list_t *list, type_t type, double val) {
  node_t *new_start = new_node(type, val, list -> start);
  list -> start = new_start;
  if (list -> end == NULL) {
    list -> end = new_start;
  }
}

void push_end(list_t * list, type_t type, double val) {
  node_t * new_end = new_node(type, val, NULL);

  if (list -> start == NULL){
    list -> start = new_end;
  } else {
    list -> end -> next = new_end;
  }

  list -> end = new_end;
}

double pop_start(list_t * list) {
  if (is_empty_list(list)) {
    return -1;
  }

  double data = list -> start -> data;
  node_t *removed = list -> start;
  list -> start = list -> start -> next;
  free(removed);
  if (list -> start == NULL) {
    list -> end = NULL;
  }
  return data;
}
// Fim da Lista --------------------------------------------------------------------------------- //

list_t* lexer(char input[]) {
  list_t *list = new_list();

  for (int i = 0; input[i]; i++) {
    char c = input[i];

    if (c >= '0' && c <= '9') {
      double num = atof(&input[i]);
      push_end(list, Number, num);
    } else if (c == '+') {
      push_end(list, Add, 0);
    } else if (c == '-') {
      push_end(list, Subtract, 0);
    } else if (c == '*') {
      push_end(list, Multiply, 0);
    } else if (c == '/') {
      push_end(list, Divide, 0);
    } else if (c == '(') {
      push_end(list, OpenParen, 0);
    } else if (c == ')') {
      push_end(list, CloseParen, 0);
    } else if (c == ' ') {
      // no-op
    } else {
      drop_list(list);
      return NULL;
    }
  }

  return list;
}

int main() {
  char input[] = "4 * 7 + 3 - (2/4 + 9)";
  list_t *list = lexer(input);

  if (list) {
    print_list(list);
    drop_list(list);
  }
  return 0;
}
