// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Yash Pandey	             2021A7PS0661P
// Achinthya Hebbar S	     2021A7PS1457P
// Ujjwal Aggarwal	         2021A7PS2427P
// Hardik Gupta 	         2021A7PS2421P
// Agrawal Vansh Anil        2021A7PS2215P

#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isDefault(struct Variable var) { return (var.flag == 0 && var.val == 0); }

struct Node *createLLNode(struct Variable data)
{
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  if (newNode == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

void insertAtBeginning(struct LinkedListArray *linkedList,
                       struct Variable var)
{

  // Create a new node
  struct Node *newNode = createLLNode(var);

  // Set the next pointer of the new node to point to the current head
  newNode->next = linkedList->head;

  // Update the head to point to the new node
  linkedList->head = newNode;

  // INCREASE LENGTH
  linkedList->length += 1;
}

bool appendVarToSet(struct LinkedListSet *set, struct Variable element)
{
  if (element.val == CARRIAGE_RETURN)
  {
    return false;
  }

  if (set->booleanArr[element.val] == true)
  {
    // Element already exists in the set
    // printf("ELE ALREADY EXISTS\n");
    return false;
  }

  // INSERT INTO LINKED LIST
  insertAtBeginning(set->linkedList, element);

  // SET IN BOOLEAN ARRAY
  set->booleanArr[element.val] = true;
  return true;
}

bool containsEPS(struct LinkedListSet *set) { return set->booleanArr[TK_EPS]; }

bool appendSetToSet(struct LinkedListSet *destinationSet,
                    struct LinkedListSet *sourceSet)
{
  // to mark whether a change has taken place
  bool changed = false;
  struct Variable var;
  var.flag = 0;

  for (int i = 0; i < NUM_TERMINALS; ++i)
  {

    if ((i != TK_EPS) && (destinationSet->booleanArr[i] == false) &&
        (sourceSet->booleanArr[i] == true))
    {
      changed = true;

      // INSERT INTO LINKED LIST
      var.val = i;
      insertAtBeginning(destinationSet->linkedList, var);

      // SET IN BOOLEAN ARRAY
      destinationSet->booleanArr[i] = true;
    }
  }
  return changed;
}

void fprintSetWithIndex(FILE *cfile, struct LinkedListSet *set, int setIndex,
                        int flag, int firstorfollow)
{
  char str[10];
  // to mark the type of set
  if (firstorfollow)
  {
    strcpy(str, "First");
  }
  else
  {
    strcpy(str, "Follow");
  }

  if (flag == 1)
  {
    fprintf(cfile, "%s(%s): ", NonTerminalToString((enum NonTerminals)setIndex),
            str);
    for (int i = 0; i < NUM_TERMINALS; ++i)
    {
      // Assuming elements are integers for simplicity
      if (set->booleanArr[i] == true)
      {
        fprintf(cfile, "%s, ", TokenToString((enum Tokentype)i));
      }
    }
    fprintf(cfile, "\n");
  }
  else
  {
    fprintf(cfile, "%s(%s): ", TokenToString((enum Tokentype)setIndex), str);
    for (int i = 0; i < NUM_TERMINALS; ++i)
    {
      // Assuming elements are integers for simplicity
      if (set->booleanArr[i])
      {
        fprintf(cfile, "%s, ", TokenToString((enum Tokentype)i));
      }
    }
    fprintf(cfile, "\n");
  }
}

void printFFSetsTable(FILE *cfile, struct Sets **sets_for_all)
{
  for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
  {
    fprintSetWithIndex(cfile, sets_for_all[i]->firstSets, i, 1, 1);
    fprintSetWithIndex(cfile, sets_for_all[i]->followSets, i, 1, 0);
    fprintf(cfile, "\n");
  }
}

struct LinkedListSet *initialiseLLSet()
{
  struct LinkedListSet *set =
      (struct LinkedListSet *)malloc(sizeof(struct LinkedListSet));
  memset(set->booleanArr, false, sizeof(set->booleanArr));

  set->linkedList =
      (struct LinkedListArray *)malloc(sizeof(struct LinkedListArray));
  set->linkedList->head = NULL;
  set->linkedList->length = 0;

  return set;
}

struct Sets *initializeSets()
{
  struct Sets *newSets = (struct Sets *)malloc(sizeof(struct Sets));

  // Initialize firstSets
  newSets->firstSets = initialiseLLSet();

  // Initialize followSets
  newSets->followSets = initialiseLLSet();

  return newSets;
}

bool isEmpty(struct LinkedListSet *destinationSet)
{
  // DIRECTLY CHECK LINKED LIST INSTEAD OF ITERATING THROUGH BOOLEAN ARRAY
  if (destinationSet->linkedList->length == 0)
  {
    return true;
  }

  return false;
}

int recomputeSetIndex(struct Variable var)
{
  int set_index = var.val;
  if (var.flag == 0)
  {
    set_index += NUM_NON_TERMINALS;
  }
  return set_index;
}

void computeFollowSet(FirstAndFollow *sets_for_all,
                      struct GrammarRule *productions)
{

  // ADD TK_EOF [END OF INPUT MARKER] to follow set of NT_PROGRAM (start symbol)
  struct Variable eof;
  eof.val = TK_EOF;
  eof.flag = 0;
  appendVarToSet(sets_for_all[NT_PROGRAM]->followSets, eof);

  // WHILE THERE IS A CHANGE IN SOMEONE'S FOLLOW SET
  bool changed = true, res = false;
  while (changed == true)
  {
    changed = false;

    for (int nonTerminal = 0; nonTerminal < NUM_NON_TERMINALS;
         nonTerminal += 1)
    {
      int numberOfRules = productions[nonTerminal].numProductions;
      for (int j = 0; j < numberOfRules; j++)
      {
        // GET THE PRODUCTION
        struct Variable *production = productions[nonTerminal].rules[j];

        // GET THE LENGTH OF THE PRODUCTION
        int length = sizeof(productions[nonTerminal].rules[j]) /
                     sizeof(productions[nonTerminal].rules[j][0]);
        for (int index1 = 0; index1 < length; index1++)
        {
          // IF IT IS A TERMINAL: NO FOLLOW SET: SKIP
          if (production[index1].flag == 0)
          {
            continue;
          }

          // If A -> αBβ, then Follow(B) = Follow(B) ∪ (First(β) - {ε})
          int index2 = index1 + 1;
          for (index2 = index1 + 1; index2 < length; index2++)
          {
            // printf("%d %d %d\n", nonTerminal, index1, index2);
            // TERMINAL: INSTEAD OF ITERATING THROUGH WHOLE SET; ADD JUST THE
            // TERMINAL
            if (production[index2].flag == 0)
            {
              res = appendVarToSet(
                  sets_for_all[production[index1].val]->followSets,
                  production[index2]);
              changed = changed || res;
              // CANNOT CONTAIN EPSILON: NO FURTHER FIRST SETS ARE ADDED
              break;
            }

            // NON TERMINAL: ADD ITS FIRST SET
            res =
                appendSetToSet(sets_for_all[production[index1].val]->followSets,
                               sets_for_all[production[index2].val]->firstSets);
            changed = changed || res;

            // NO EPSILON: NO MORE FIRST SETS ADDED FROM THIS PRODUCTION
            if (!containsEPS(sets_for_all[production[index2].val]->firstSets))
            {
              break;
            }
          }

          // EPSILON UPTO THE LAST SYMBOL AFTER INDEX1
          if (index2 == length || isDefault(production[index2]))
          {
            res =
                appendSetToSet(sets_for_all[production[index1].val]->followSets,
                               sets_for_all[nonTerminal]->followSets);

            changed = changed || res;
          }

          // REMOVE EPSILON FROM FOLLOW SET: NO FOLLOW SET CAN HAVE EPSILON
          sets_for_all[production[index1].val]->followSets->booleanArr[TK_EPS] =
              false;
        }
      }
    }
  }
}

void computeFirstSetNT(struct GrammarRule *productions,
                       struct Sets **sets_for_all, int nonTerminal)
{
  // FOR EVERY PRODUCTION
  int numberOfRules = productions[nonTerminal].numProductions;
  for (int j = 0; j < numberOfRules; j++)
  {
    // GET THE PRODUCTION
    struct Variable *production = productions[nonTerminal].rules[j];

    // first we check if the rule is an epsilon rule for that non terminal
    if (production[0].val == TK_EPS)
    {
      appendVarToSet(sets_for_all[nonTerminal]->firstSets, production[0]);
      continue;
    }

    // index keeps track of how many non-terminals we have looked at so far
    int index = 0;

    // length is supposed to keep the max of non-terminals we could have to
    // check for
    int length = sizeof(productions[nonTerminal].rules[j]) /
                 sizeof(productions[nonTerminal].rules[j][0]);

    int set_index = recomputeSetIndex(production[index]);

    while (index < length && production[index].flag == 1)
    {
      // RECOMPUTE SET INDEX
      set_index = production[index].val;

      // CHECK WHETHER THE SET IS EMPTY
      if (isEmpty(sets_for_all[set_index]->firstSets))
      {
        // COMPUTE ITS FIRST SET
        computeFirstSetNT(productions, sets_for_all, production[index].val);
      }

      // ADD THE SET
      appendSetToSet(sets_for_all[nonTerminal]->firstSets,
                     sets_for_all[set_index]->firstSets);

      // I ONLY KNOW IF THE SET HAS EPSILON AFTER COMPUTING IT IF IT WAS EMPTY
      if (!containsEPS(sets_for_all[set_index]->firstSets))
      {
        break;
      }

      index++;
    }

    if (index != length || isDefault(production[index]))
    {
      // RECOMPUTE SET INDEX
      set_index = recomputeSetIndex(production[index]);

      if (nonTerminal == NT_FUN_CALL_STMT)
      {
        printf("%d\n", index);
      }

      // printf("INDEX %d APPENDED\n", index);
      appendSetToSet(sets_for_all[nonTerminal]->firstSets,
                     sets_for_all[set_index]->firstSets);
      sets_for_all[nonTerminal]->firstSets->booleanArr[TK_EPS] = 0;
    }

    else
    {
      sets_for_all[nonTerminal]->firstSets->booleanArr[TK_EPS] = 1;
    }
  }
}

void computeFirstSet(struct Sets **sets_for_all,
                     struct GrammarRule *productions)
{
  // printf("FIRST SETS\n");
  // for all the terminal first sets, we add the terminal to the first set
  // I have created the sets_for_all as an array of Sets structures. Each
  // structure holds the first and follow sets for that variable I have kept the
  // non terminals first and only then have I kept the terminals
  struct Variable var;
  var.flag = 0;

  for (int i = NUM_NON_TERMINALS; i < NUM_NON_TERMINALS + NUM_TERMINALS; i++)
  {
    // INSERT IN LINKED LIST
    var.val = i - NUM_NON_TERMINALS;
    insertAtBeginning(sets_for_all[i]->firstSets->linkedList, var);

    // SET IN BOOLEAN ARRAY
    sets_for_all[i]->firstSets->booleanArr[i - NUM_NON_TERMINALS] = true;
  }

  // FOR ALL NON TERMINALS
  for (int i = 0; i < NUM_NON_TERMINALS; i++)
  {
    // ONLY COMPUTE THHE SETS ONCE
    if (isEmpty(sets_for_all[i]->firstSets))
    {
      computeFirstSetNT(productions, sets_for_all, i);
    }
  }
}

struct Sets **initialiseSetsWhole()
{
  struct Sets **sets_for_all = (struct Sets **)malloc(
      sizeof(struct Sets *) * (NUM_NON_TERMINALS + NUM_TERMINALS));

  for (int i = 0; i < NUM_NON_TERMINALS + NUM_TERMINALS; i += 1)
  {
    sets_for_all[i] = initializeSets();
  }

  return sets_for_all;
}

struct tree_node *create_tree_node(struct Variable *data)
{
  struct tree_node *new_tree_node =
      (struct tree_node *)malloc(sizeof(struct tree_node));
  new_tree_node->data = data;
  new_tree_node->next = NULL;
  new_tree_node->head = NULL;
  new_tree_node->parent = NULL;
  return new_tree_node;
}

// this function was used for generating a visual representation for the graph
// using the pyvis library
void serialize_node(struct tree_node *node, FILE *output)
{
  if (node == NULL)
    return;

  // Serialize current node data and its relationships
  fprintf(output, "{\n");
  fprintf(output, "  \"current\": \"%p\",\n", (void *)node);

  if (node->data->flag == 1)
  {
    fprintf(
        output, "  \"data\": \"%s\",\n",
        NonTerminalToString(
            node->data->val)); // Assuming data can be represented as a string
    fprintf(output, "  \"isleaf\": \"no\",\n");
  }
  else
  {
    fprintf(
        output, "  \"data\": \"%s\",\n",
        TokenToString(
            node->data->val)); // Assuming data can be represented as a string
    fprintf(output, "  \"isleaf\": \"yes\",\n");
  }
  if (node->data->token != NULL)
  {
    fprintf(output, "  \"lexeme\": \"%s\",\n", node->data->token->lexeme);
    fprintf(output, "  \"lineNo\": \"%d\",\n", node->data->token->lineNo);
    if (node->data->token->tokenType == TK_RNUM)
    {
      fprintf(output, "  \"value\": \"%lf\",\n",
              node->data->token->doubleValue);
    }
    else if (node->data->token->tokenType == TK_NUM)
    {
      fprintf(output, "  \"value\": \"%d\",\n", node->data->token->intValue);
    }
    else
    {
      // value is "-----"
      fprintf(output, "  \"value\": \"-----\",\n");
    }
  }
  else
  {
    fprintf(output, "  \"lexeme\": \"-----\",\n");
    fprintf(output, "  \"lineNo\": \"-1\",\n");
    fprintf(output, "  \"value\": \"-----\",\n");
  }
  if (node->parent != NULL)
  {
    fprintf(output, "  \"parent\": \"%p\",\n", (void *)node->parent);
  }
  if (node->next != NULL)
  {
    fprintf(output, "  \"next\": \"%p\",\n", (void *)node->next);
  }
  fprintf(output, "  \"children\": [\n");

  struct tree_node *child = node->head;
  while (child != NULL)
  {
    serialize_node(child, output);
    child = child->next;
    if (child != NULL)
      fprintf(output, ",\n");
  }

  fprintf(output, "  ]\n");
  fprintf(output, "}\n");
}

void serialize_tree(struct tree_node *root)
{
  printf("%d", root->data->val);
  printf("Serializing tree\n");
  FILE *output = fopen("tree_structure.json", "w");
  if (output == NULL)
  {
    printf("Error opening file\n");
    return;
  }

  serialize_node(root, output);

  fclose(output);
}

// function to add a tree_node to the linkedlist which functions as the children
// of the parse tree the tree_nodes are added to the head (they are in reverse
// order)
void add_tree_node(struct tree_node *parent, struct tree_node *child)
{
  child->next = parent->head;
  parent->head = child;
  child->parent = parent;
}

// function to find the next non terminal for a given node
struct tree_node *nextNonTerminal(struct tree_node *current)
{
  if (current == NULL)
  {
    return NULL;
  }

  while (current->next != NULL && current->data->flag == 0)
  {
    current = current->next;
  }

  if (current->data->flag == 1)
  {
    return current;
  }
  while (current->parent != NULL && current->parent->next == NULL)
  {
    current = current->parent;
  }
  if (current->parent == NULL)
  {
    return NULL;
  }
  return nextNonTerminal(current->parent->next);
}

// function which takes the structure described in the first few comments as
// input and the tree_node where the rule has to be added
struct tree_node *repeated_add(struct tree_node *parent, struct Variable *nt,
                               struct Variable **rule)
{

  if (parent->data->val != nt->val)
  {
    return nextNonTerminal(parent->head);
  }

  for (int var = MAX_VARS - 1; var >= 0; var -= 1)
  {
    if (rule[var] == NULL)
    {
      continue;
    }

    add_tree_node(parent, create_tree_node(rule[var]));
  }

  return nextNonTerminal(parent->head);
}
struct tree_node *add_to_tree(struct Variable *nt, struct Variable **rule,
                              struct tree_node *parent)
{
  return repeated_add(parent, nt, rule);
}

void printNodeDetails(struct tree_node *node, FILE *outfile)
{
  // error handling
  if (node == NULL)
  {
    // printf("NULL NODE ENCOUNTERED");
    return;
  }
  if (node->data == NULL)
  {
    // printf("DATA IS NULL");
    return;
  }
  // fields to be printed
  char *parentNodeSymbol = NULL;
  int isLeaf = (node->data->flag == 0);
  char *nodeSymbol = "LEAF";
  char *lexeme = NULL;
  int lineNo = -1;

  if (node->parent == NULL)
  {
    parentNodeSymbol = "ROOT";
  }
  else
  {
    parentNodeSymbol = NonTerminalToString(node->parent->data->val);
  }

  if (isLeaf)
  {
    if (node->data->token != NULL)
    {
      lexeme = node->data->token->lexeme;
    }
  }
  else
  {
    // nodeSymbol = (char *)malloc(20 * sizeof(char));
    nodeSymbol = NonTerminalToString(node->data->val);
  }

  if (node->data->token == NULL)
  {
    // its not assigned a token
    fprintf(outfile,
            "(Lexeme Current Node)%-20s (Line No)%-5d (Token Name)%-20s (Value "
            "if number)%-20s (Parent Node Symbol)%-20s (IS leaf node)%-3s "
            "(Node symbol)%-20s\n",
            "-----", lineNo, isLeaf ? TokenToString(node->data->val) : "-----",
            "-----", parentNodeSymbol, isLeaf ? "yes" : "no", nodeSymbol);
    return;
  }

  lineNo = node->data->token->lineNo;

  if (node->data->token->tokenType == TK_RNUM)
  {
    fprintf(outfile,
            "(Lexeme Current Node)%-20s (Line No)%-5d (Token Name)%-20s (Value "
            "if number)%-20e (Parent Node Symbol)%-20s (IS leaf node)%-3s "
            "(Node symbol)%-20s\n",
            lexeme, lineNo, TokenToString(node->data->token->tokenType),
            node->data->token->doubleValue, parentNodeSymbol, "yes",
            nodeSymbol);
  }
  else if (node->data->token->tokenType == TK_NUM)
  {
    fprintf(outfile,
            "(Lexeme Current Node)%-20s (Line No)%-5d (Token Name)%-20s (Value "
            "if number)%-20d (Parent Node Symbol)%-20s (IS leaf node)%-3s "
            "(Node symbol)%-20s\n",
            lexeme, lineNo, TokenToString(node->data->token->tokenType),
            node->data->token->intValue, parentNodeSymbol, "yes", nodeSymbol);
  }
  else
  {
    fprintf(outfile,
            "(Lexeme Current Node)%-20s (Line No)%-5d (Token Name)%-20s (Value "
            "if number)%-20s (Parent Node Symbol)%-20s (IS leaf node)%-3s "
            "(Node symbol)%-20s\n",
            lexeme, lineNo, TokenToString(node->data->token->tokenType),
            "-----", parentNodeSymbol, "yes", nodeSymbol);
  }
}

void inorderTraversal(struct tree_node *node, FILE *outfile)
{
  if (node == NULL)
  {
    // printf("NULL NODE ENCOUNTERED");
    return;
  }

  if (node->head != NULL)
  {
    // printf("RECURSING");
    inorderTraversal(node->head, outfile);
  }

  // // Print the current (parent) node
  printNodeDetails(node, outfile);

  // For the remaining children
  struct tree_node *sibling = node->head ? node->head->next : NULL;
  while (sibling != NULL)
  {
    inorderTraversal(sibling,
                     outfile); // Assuming lexeme as the non-terminal symbol.
    sibling = sibling->next;
  }
}

void printParseTree(parseTree *PT, char *outfile)
{
  FILE *file = fopen(outfile, "w");
  if (!file)
  {
    printf("Failed to open the file %s for writing.\n", outfile);
    return;
  }

  if (PT && PT->root)
  {

    if (PT->root)
    {
      // printf("STARTING INORDER TRAVERSAL\n");
      inorderTraversal(PT->root, file);
    }
  }

  fclose(file);
}

void printRule(enum NonTerminals nt, struct Variable *ruleArray)
{

  printf("%s ===> ", NonTerminalToString(nt));

  for (int i = 0; i < MAX_VARS; i++)
  {
    if (ruleArray[i].val == 0 && ruleArray[i].flag == 0)
    {
      break;
    }
    if (ruleArray[i].flag == 0)
    {
      printf("%s ", TokenToString(ruleArray[i].val));
    }
    else
    {
      printf("%s ", NonTerminalToString(ruleArray[i].val));
    }
  }

  printf("\n");
}
void printParsingTable(struct ParsingTable *pt)
{
  if (pt == NULL)
  {
    printf("ParsingTable pointer is NULL\n");
    return;
  }
  for (int i = 0; i < NUM_NON_TERMINALS; i++)
  {
    for (int j = 0; j < NUM_TERMINALS; j++)
    {

      printf("Cell[%s][%s]:", NonTerminalToString(i), TokenToString(j));
      if (pt->table[i][j] != NULL)
      {
        printRule(i, pt->table[i][j]);
      }
      else
      {
        printf("ERROR\n");
      }
    }
  }
}
void insert(enum NonTerminals nt, enum Tokentype terminal,
            struct Variable *rule, struct ParsingTable *PT)
{
  PT->table[(int)(nt)][(int)terminal] = rule;
}

void addSyn(struct ParsingTable *PT, struct Sets **sets_for_all,
            int nonTerminal, int *synchSet, int defaultSynchSetSize)
{
  // SYN INDICATING RULE
  struct Variable *rule = (struct Variable *)malloc(sizeof(struct Variable));
  rule[0].val = -1;

  struct Node *current =
      sets_for_all[nonTerminal]->followSets->linkedList->head;

  // FOR ALL TERMINALS IN FOLLOW OF NONTERMINAL
  while (current != NULL)
  {
    // INSERT INTO PARSING TABLE
    insert(nonTerminal, current->data.val, rule, PT);
    current = current->next;
  }

  // FOR ALL IN DEFAULT SYNC SET
  for (int i = 0; i < defaultSynchSetSize; i += 1)
  {
    insert(nonTerminal, synchSet[i], rule, PT);
  }
}

void createParseTable(struct ParsingTable *PT, struct GrammarRule *productions,
                      struct Sets **sets_for_all, int *synchSet,
                      int defaultSynchSetSize)
{
  for (int nt = 0; nt < NUM_NON_TERMINALS; nt++)
  {
    // ADD THE SYN INDICATOR FOR ALL TERMINALS IN FOLLOW OF NT
    addSyn(PT, sets_for_all, nt, synchSet, defaultSynchSetSize);

    // printf("NUM PRODUCTIONS: %d\n", productions[nt].numProductions);
    for (int i = 0; i < productions[nt].numProductions; i++)
    {
      // GET CURRENT RULE
      struct Variable *rule = productions[nt].rules[i];

      // EPSILON RULE
      if (rule[0].val == TK_EPS)
      {
        struct Node *current = sets_for_all[nt]->followSets->linkedList->head;

        while (current != NULL)
        {

          // printf("ADDING TO PARSE TREE: %d %d", current->data.val,
          // rule[0].val);
          insert(nt, current->data.val, rule, PT);
          current = current->next;
        }

        continue;
      }

      else
      {

        int var = 0;
        for (; var < MAX_VARS && !(isDefault(rule[var])); var += 1)
        {

          int set_index = rule[var].val;
          if (rule[var].flag == 0)
          {
            set_index += NUM_NON_TERMINALS;
          }
          struct Node *current =
              sets_for_all[set_index]->firstSets->linkedList->head;

          while (current != NULL)
          {
            // DONT ADD TK_EPS
            if (current->data.val == TK_EPS)
            {
              current = current->next;
              continue;
            }
            insert(nt, current->data.val, rule, PT);
            current = current->next;
          }

          if (!(containsEPS(sets_for_all[set_index]->firstSets)))
          {
            break;
          }
        }

        if (var == MAX_VARS || isDefault(rule[var]))
        {
          // ADD THE RULE IN FOLLOW OF THE VAR [EPSILON RULE]
          struct Node *current = sets_for_all[nt]->followSets->linkedList->head;

          while (current != NULL)
          {
            insert(nt, current->data.val, rule, PT);
            current = current->next;
          }
        }
      }
    }
  }

  return;
}
char *NonTerminalToString(enum NonTerminals nonTerminal)
{
  switch (nonTerminal)
  {
  case NT_PROGRAM:
    return "program";
  case NT_MAIN_FUNCTION:
    return "mainFunction";
  case NT_OTHER_FUNCTIONS:
    return "otherFunctions";
  case NT_FUNCTION:
    return "function";
  case NT_INPUT_PAR:
    return "input_par";
  case NT_OUTPUT_PAR:
    return "output_par";
  case NT_PARAMETER_LIST:
    return "parameter_list";
  case NT_DATA_TYPE:
    return "dataType";
  case NT_PRIMITIVE_DATA_TYPE:
    return "primitiveDatatype";
  case NT_CONSTRUCTED_DATA_TYPE:
    return "constructedDatatype";
  case NT_REMAINING_LIST:
    return "remaining_list";
  case NT_STMTS:
    return "stmts";
  case NT_TYPE_DEFINITIONS:
    return "typeDefinitions";
  case NT_ACTUAL_OR_REDEFINED:
    return "actualOrRedefined";
  case NT_TYPE_DEFINITION:
    return "typeDefinition";
  case NT_FIELD_DEFINITIONS:
    return "fieldDefinitions";
  case NT_FIELD_DEFINITION:
    return "fieldDefinition";
  case NT_FIELD_TYPE:
    return "fieldType";
  case NT_MORE_FIELDS:
    return "moreFields";
  case NT_DECLARATIONS:
    return "declarations";
  case NT_DECLARATION:
    return "declaration";
  case NT_GLOBAL_OR_NOT:
    return "global_or_not";
  case NT_OTHER_STMTS:
    return "otherStmts";
  case NT_STMT:
    return "stmt";
  case NT_ASSIGNMENT_STMT:
    return "assignmentStmt";
  case NT_SINGLE_OR_REC_ID:
    return "singleOrRecId";
  case NT_OPTION_SINGLE_CONSTRUCTED:
    return "option_single_constructed";
  case NT_ONE_EXPANSION:
    return "oneExpansion";
  case NT_MORE_EXPANSIONS:
    return "moreExpansions";
  case NT_FUN_CALL_STMT:
    return "funCallStmt";
  case NT_OUTPUT_PARAMETERS:
    return "outputParameters";
  case NT_INPUT_PARAMETERS:
    return "inputParameters";
  case NT_ITERATIVE_STMT:
    return "iterativeStmt";
  case NT_CONDITIONAL_STMT:
    return "conditionalStmt";
  case NT_ELSE_PART:
    return "elsePart";
  case NT_IO_STMT:
    return "ioStmt";
  case NT_ARITHMETIC_EXPRESSION:
    return "arithmeticExpression";
  case NT_EXP_PRIME:
    return "expPrime";
  case NT_TERM:
    return "term";
  case NT_TERM_PRIME:
    return "termPrime";
  case NT_FACTOR:
    return "factor";
  case NT_HIGH_PRECEDENCE_OPERATORS:
    return "highPrecedenceOperators";
  case NT_LOW_PRECEDENCE_OPERATORS:
    return "lowPrecedenceOperators";
  case NT_BOOLEAN_EXPRESSION:
    return "booleanExpression";
  case NT_VAR:
    return "var";
  case NT_LOGICAL_OP:
    return "logicalOp";
  case NT_RELATIONAL_OP:
    return "relationalOp";
  case NT_RETURN_STMT:
    return "returnStmt";
  case NT_OPTIONAL_RETURN:
    return "optionalReturn";
  case NT_ID_LIST:
    return "idList";
  case NT_MORE_IDS:
    return "moreIds";
  case NT_DEFINETYPE_STMT:
    return "definetypeStmt";
  case NT_A:
    return "A";

  default:
  {
    char *result = (char *)malloc(50);
    snprintf(result, 50, "Unknown NonTerminal - %d", nonTerminal);
    return result;
  }
  }
}

void printStack(struct stack *st, FILE *errors)
{
  for (int i = 0; i <= st->top; i++)
  {
    if ((st->stack[i])->flag == 1)
    {
      // printf("%s ", NonTerminalToString((st->stack[i])->val));
      fprintf(errors, "%s ", NonTerminalToString((st->stack[i])->val));
    }

    else
    {
      // printf("%s ", TokenToString((st->stack[i])->val));
      fprintf(errors, "%s ", TokenToString((st->stack[i])->val));
    }
  }
  // printf("\n");
  fprintf(errors, "\n");
}
bool isEmptyStack(struct stack *st)
{
  return st->top == -1;
}

struct Variable *peek(struct stack *st)
{

  if (!isEmptyStack(st))
  {
    return st->stack[st->top];
  }
  else
  {
    printf("Stack is empty\n");
    return NULL;
  }
}

bool onlyContainsEOF(struct stack *st)
{
  struct Variable *topStack = peek(st);
  return st->top == 0 && topStack->val == TK_EOF && topStack->flag == 0;
}

bool isFull(struct stack *st)
{
  if (st->top == st->MAX - 1)
  {
    st->MAX += 128;
    st->stack = (struct Variable **)realloc(
        st->stack, 2 * st->MAX * sizeof(struct Variable *));
    return true;
  }
  return false;
}

void push(struct stack *st, struct Variable *data)
{
  // CHECKS IF FULL AND REALLOCATES
  isFull(st);

  st->stack[++st->top] = data;
}

struct Variable *pop(struct stack *st)
{

  if (isEmptyStack(st))
  {
    printf("Stack is empty\n");
    return NULL;
  }

  return st->stack[st->top--];
}

struct Variable *createCopy(struct Variable var)
{
  struct Variable *copy = (struct Variable *)malloc(sizeof(struct Variable));
  copy->flag = var.flag;
  copy->val = var.val;
  copy->token = NULL;
  // no need ot token
  return copy;
}

int parseInputSourceCode(struct SymbolTableEntry *token,
                         struct ParsingTable *pt, struct stack *st,
                         lexicalAnalyser LA, struct tree_node *parent,
                         bool skipError, struct tree_node **parentpointer,
                         FILE *errors, int toPrint)
{

  // GET THE TOKENTYPE OF CURRENT TOKEN
  enum Tokentype a = token->tokenType;

  // GET THE VARIABLE AT THE TOP OF STACK
  struct Variable *X = st->stack[st->top];

  // BOTH ARE TERMINALS AND MATCH
  if (X->val == a && X->flag == 0)
  {
    // TOKEN BEING SET [NO ERROR]
    X->token = token;

    // POP THE TOKEN
    pop(st);

    // CORRECT TOKEN; GET NEXT AND STOP SKIPPING ERRORS IF WERE SKIPPING
    return 1;
  }

  // TOKEN AND MISMATCH
  else if (X->flag == 0)
  {

    // IF TO PRINT (NOT PRINTING INTO A FILE)
    if (toPrint)
    {
      // CALL ERROR FUNCTION
      fprintf(errors,
              "Line no. %-5d Error: The token %s for lexeme %s  does not match "
              "with the expected token %s\n",
              LA->lineNo, TokenToString(a), token->lexeme, TokenToString(X->val));
    }

    // TOKEN NOT BEING SET
    pop(st);

    // WRONG TOKEN; STAY AT THIS; ASSUME ALL TOKENS TO BE IN SYNCH SET OF ALL TOKENS
    return 0;
  }

  // ERROR: START SKIPPING THE INPUT
  else if (pt->table[X->val][a] == NULL)
  {
    if (!skipError && toPrint)
    {

      fprintf(errors,
              "Line no. %-5d Error: Invalid token %s encountered with value %s "
              "stack top %s\n",
              LA->lineNo, TokenToString(a), token->lexeme,
              NonTerminalToString(X->val));
    }

    // go and get the next token [RETURN -1 INSTEAD OF 0]

    // ERROR ENTRY; GET NEXT AND START SKIPPING; DISCARD INPUT UNTIL SYN OR VALID
    return -1;
  }
  // SYN
  else if (pt->table[X->val][a][0].val == -1)
  {
    // SYN TOKEN; POP THE NONTERMINAL

    // ERROR IF NOT SKIPPING ERRORS; REPORT THIS ERROR
    if (!skipError && toPrint)
    {
      fprintf(errors,
              "Line no. %-5d Error: Invalid token %s encountered with value %s "
              "stack top %s\n",
              LA->lineNo, TokenToString(a), token->lexeme,
              NonTerminalToString(X->val));
    }

    // TOKEN NOT BEING SET
    pop(st);

    // SYNCH; STAY AT THIS; IF SKIPPING; STOP SKIPPING; CONTINUE FROM  SYN TOKEN
    return 0;
  }
  else
  {
    // GET THE RULE
    struct Variable *topStack = pop(st);
    struct Variable **copyRule =
        (struct Variable **)malloc(sizeof(struct Variable *) * MAX_VARS);

    for (int var = MAX_VARS - 1; var >= 0; var -= 1)
    {

      if (isDefault(pt->table[X->val][a][var]))
      {
        copyRule[var] = NULL;
        continue;
      }

      // assign epsilon or anything other than default
      struct Variable *copy = createCopy(pt->table[X->val][a][var]);
      copyRule[var] = copy;

      if (pt->table[X->val][a][var].val == TK_EPS &&
          pt->table[X->val][a][var].flag == 0)
      {
        break;
      }

      // MAY NOT PUSH COPY BUT ALWAYS ADD TO RULE
      push(st, copy);
    }

    // PASS TO TREE & GET CURRENT NODE`
    parent = add_to_tree(topStack, copyRule, parent);
    *parentpointer = parent;

    // USED A RULE BUT NOT POPPED TOKEN; STAY AT THIS TOKEN
    return 0;
  }
}

struct stack *initialiseStack()
{
  struct stack *stack = (struct stack *)malloc(sizeof(struct stack));

  stack->MAX = STACK_INITIAL_SIZE;
  stack->top = -1;
  stack->stack = (struct Variable **)malloc(sizeof(struct Variable *) *
                                            STACK_INITIAL_SIZE);
  return stack;
}

FirstAndFollow *computeFirstAndFollow(struct GrammarRule *productions)
{
  FirstAndFollow *sets = initialiseSetsWhole();
  computeFirstSet(sets, productions);
  // printf("FIRST SET COMPUTED\n");

  computeFollowSet(sets, productions);
  // printf("FOLLOW SET COMPUTED\n");

  return sets;
}

parseTree *create_tree(struct Variable *init)
{
  parseTree *tree = (parseTree *)malloc(sizeof(parseTree));

  tree->root = create_tree_node(init);

  return tree;
}

void initialiseStackItems(struct stack *stack, struct Variable *init)
{
  push(stack, createCopy((struct Variable){TK_EOF, 0}));
  push(stack, init);
}

void freeTreeNode(struct tree_node *node)
{
  if (node == NULL)
  {
    return;
  }

  free(node->data);

  freeTreeNode(node->next);

  freeTreeNode(node->head);

  free(node);
}

// Function to free the entire parse tree
void freeTree(struct parseTree *tree)
{
  if (tree == NULL || tree->root == NULL)
  {
    return;
  }

  freeTreeNode(tree->root);

  tree->root = NULL;

  free(tree);
}

void freeAll(parseTree *tree, lexicalAnalyser LA, struct SymbolTableEntry *token, struct stack *stack)
{
  freeTree(tree);
  free(LA->bufferArray);
  free(LA);
  free(token);
  free(stack->stack);
  free(stack);
}

void print_and_parse_tree(char *testfile, char *outputfile,
                          FirstAndFollow *sets, struct ParsingTable *PT,
                          Grammar G, int toPrint)
{

  // VARIABLE FOR THE START SYMBOL
  struct Variable *init = createCopy((struct Variable){NT_PROGRAM, 1});

  // INITIALISE A PARSE TREE
  parseTree *tree = create_tree(init);

  // INITIALLY ADD TO ROOT NODE
  struct tree_node *node_to_add_to = tree->root;
  struct tree_node **parentpointer =
      (struct tree_node **)malloc(sizeof(struct tree_node *));
  *parentpointer = node_to_add_to;

  // INITIALISE A STACK
  struct stack *stack = initialiseStack();

  // READ THE INPUT TEST FILE
  FILE *file = readTestFile(testfile);

  // TWIN BUFFER
  twinBuffer bufferArray = initialiseTwinBuffer(file);

  // COMPUNDED DATASTRUCTURE HOLDING THE BUFFER, LINENO, STATE, BEGIN, FORWARD
  lexicalAnalyser LA = initialiseLA(bufferArray);

  // HOLD CURRENT TOKEN
  struct SymbolTableEntry *token;

  int res = 0;
  bool skip_error = false;
  getStream(bufferArray);

  initialiseStackItems(stack, init);
  FILE *errors;
  if (toPrint)
  {
    errors = fopen("errors_file.txt", "w");
    if (errors == NULL)
    {
      printf("Error opening file errors\n");
      return;
    }
  }

  while ((token = getNextToken(LA)))
  {
    if (token->tokenType == LEXICAL_ERROR)
    {
      // LEXICAL ERRORS
      if (toPrint)
      {
        fprintf(errors, "Line no. %-5d Error: %-30s\n", LA->lineNo,
                token->lexeme);
      }
      continue;
    }
    if (!(token->tokenType == TK_COMMENT))
    {

      while ((!isEmptyStack(stack)) &&
             ((res = parseInputSourceCode(token, PT, stack, LA, node_to_add_to, skip_error, parentpointer, errors, toPrint)) == 0))
      {
        skip_error = false;

        if (*parentpointer != NULL)
        {
          node_to_add_to = *parentpointer;
        }
      }

      if (res == -1)
      {
        skip_error = true;
      }
      if (res == 1)
      {
        skip_error = false;
      }
      if (*parentpointer != NULL)
      {
        node_to_add_to = *parentpointer;
      }
    }
  }
  if (toPrint)
  {

    fclose(errors);
    errors = fopen("errors_file.txt", "r");

    if (errors == NULL)
    {
      printf("Failed to open the file.\n");
      return;
    }

    char error_line[256];
    while (fgets(error_line, sizeof(error_line), errors) != NULL)
    {
      printf("%s", error_line);
    }

    fclose(errors);

    printParseTree(tree, outputfile);
    serialize_tree(tree->root);

    if (onlyContainsEOF(stack))
    {
      printf(
          "SYNTAX ANALYSIS COMPLETE\n");
    }
    else
    {
      printf("THIS CODE IS SYNTACTICALLY INCORRECT.");
    }
  }

  freeAll(tree, LA, token, stack);

  return;
}
