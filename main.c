#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum type {
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

int op_priority(type_t type) {
  switch (type) {
    case Add:
    case Subtract:
      return 1;
    case Multiply:
    case Divide:
      return 2;
    default:
      return -1;

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
  if (node -> type == Number) {
    printf("%5.5f\n", node -> data);
  } else {
    printf("%s\n", type_name(node -> type));
  }
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

node_t* pop_start(list_t * list) {
  if (is_empty_list(list)) {
    return NULL;
  }

  node_t *removed = list -> start;
  list -> start = list -> start -> next;
  if (list -> start == NULL) {
    list -> end = NULL;
  }
  return removed;
}
// Fim da Lista --------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------------------- //
//                                       Definição da Pilha                                       //
// ---------------------------------------------------------------------------------------------- //

typedef struct stack {
  list_t *list;
} stack_t;

stack_t* new_stack() {
  stack_t *stack =  (stack_t*)malloc(sizeof(stack_t));
  stack -> list = new_list();
  return stack;
}

void add_stack(stack_t *stack, type_t type, double val) {
  push_start(stack -> list, type, val);
}

node_t* remove_stack(stack_t *stack) {
  return pop_start(stack -> list);
}

type_t peek(stack_t *stack) {
  return stack -> list -> start -> type;
}

type_t remove_stack_type(stack_t *stack) {
  node_t *removed = remove_stack(stack);
  type_t t = removed -> type;
  free(removed);
  return t;
}

double remove_stack_data(stack_t *stack) {
  node_t *removed = remove_stack(stack);
  double data = removed -> data;
  free(removed);
  return data;
}

void drop_stack(stack_t *stack) {
  drop_list(stack -> list);
  free(stack);
}

int is_stack_empty(stack_t *stack) {
  return is_empty_list(stack -> list);
}

// Fim da pilha --------------------------------------------------------------------------------- //

// ---------------------------------------------------------------------------------------------- //
//                                       Definição do fila                                        //
// ---------------------------------------------------------------------------------------------- //

typedef struct queue {
  list_t *list;
} queue_t;

queue_t* new_queue() {
  queue_t * queue = (queue_t*)malloc(sizeof(queue_t));
  queue -> list = new_list();
  return queue;
}

void add_queue(queue_t *queue, type_t type, double val) {
  push_end(queue -> list, type, val);
}

node_t* remove_queue(queue_t *queue) {
  return pop_start(queue -> list);
}

type_t remove_queue_type(queue_t *queue) {
  node_t *node = pop_start(queue -> list);
  type_t type = node -> type;
  free(node);
  return type;
}

double remove_queue_data(queue_t *queue) {
  node_t *node = pop_start(queue -> list);
  double data = node -> data;
  free(node);
  return data;
}

bool is_queue_empty(queue_t *queue) {
  return is_empty_list(queue -> list);
}

// Fim da pilha --------------------------------------------------------------------------------- //

list_t* lexer(char input[]) {
  list_t *list = new_list();

  for (int i = 0; input[i]; i++) {
    char c = input[i];

    if (c >= '0' && c <= '9') {
      double num = atoll(&input[i]);
      push_end(list, Number, num);
      while ((input[i+1] >= '0' && input[i+1] <= '9')) i++;
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

queue_t* infix2postfix(list_t *infix) {
  stack_t *operadores = new_stack();
  queue_t *postfix = new_queue();

  node_t *current = pop_start(infix);
  while (current) {
    switch (current -> type) {
      case Number:
        add_queue(postfix, current -> type, current -> data);
        break;

      case Add:
      case Subtract:
      case Divide:
      case Multiply: {
        while (
          !is_stack_empty(operadores) &&
          op_priority(peek(operadores)) >= op_priority(current -> type)
        ) {
          type_t op = remove_stack_type(operadores);
          add_queue(postfix, op, 0);
        }
      }

      case OpenParen:
        add_stack(operadores, current -> type, 0);
        break;

      case CloseParen: {
        type_t op = remove_stack_type(operadores);
        while (op != OpenParen) {
          add_queue(postfix, op, 0);
          op = remove_stack_type(operadores);
        }
      }
    }

    free(current);
    current = pop_start(infix);
  }

  while (!is_stack_empty(operadores)) {
    type_t type = remove_stack_type(operadores);
    add_queue(postfix, type, 0);
  }

  drop_stack(operadores);
  return postfix;
}

double execute(list_t *expr) {
  stack_t *values = new_stack();

  node_t *current = expr -> start;
  while (current) {
    double result;

    if (current -> type == Number) {
      result = current -> data;
    } else {
      double n1 = remove_stack_data(values);
      double n2 = remove_stack_data(values);

      switch (current -> type) {
        case Divide:   result = n2 / n1; break;
        case Multiply: result = n2 * n1; break;
        case Add:      result = n2 + n1; break;
        case Subtract: result = n2 - n1; break;
        default: exit(3);
      }
    }

    add_stack(values, Number, result);

    current = current -> next;
  }

  double result = remove_stack_data(values);
  drop_stack(values);
  return result;
}

int main() {
  char input[100];
  printf("Digite uma expressão: ");
  scanf("%[^\n]s", input);

  list_t *infixa = lexer(input);
  if (!infixa) {
    fprintf(stderr, "Lexer Error.\n");
    return 1;
  }

  queue_t *posfixa = infix2postfix(infixa);
  if (!posfixa) {
    fprintf(stderr, "Infix to Postfix Error.\n");
    return 2;
  }

  printf("Result: %f\n", execute(posfixa -> list));
  // print_list(posfixa -> list);
  drop_list(infixa);
  drop_list(posfixa -> list);
  free(posfixa);

  return 0;
}
