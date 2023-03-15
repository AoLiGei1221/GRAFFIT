/**
 * CSC A48 - Intro to Computer Science II, Summer 2021
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * Assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO.
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 * (c) 2021 Charles Xu
 **/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STR_LEN 1024
#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
  //based on hint
  int distance;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
 */
User *create_user(char *name)
{
  User *new_user = (User *) calloc(1, sizeof(User));
  strcpy(new_user -> name, name);
  new_user -> friends = NULL;
  new_user -> brands = NULL;
  new_user -> visited = false;
  if(in_friend_list(allUsers, new_user)){
    free(new_user);
    return NULL;
  }
  allUsers = insert_into_friend_list(allUsers, new_user);
  return new_user;
}

/**
 * TODO: Complete this function
 * Deletes a given user.
 */
int delete_user(User *user)
{
  if(in_friend_list(allUsers, user) == 0){
    return -1;
  }
  allUsers = delete_from_friend_list(allUsers, user);
  FriendNode *p = user -> friends;
  while(p != NULL) {
    p -> user -> friends = delete_from_friend_list(p -> user -> friends, user);
    p = p -> next;
  }
  free(user);
  return 0;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
  if(in_friend_list(user -> friends, friend) == 1){
    return -1;
  }
  user -> friends = insert_into_friend_list(user -> friends, friend);
  friend -> friends = insert_into_friend_list(friend -> friends, user);
  return 0;
}

/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
  if(!in_friend_list(user -> friends, friend)){
    return -1;
  }
  user -> friends = delete_from_friend_list(user -> friends, friend);
  friend -> friends = delete_from_friend_list(friend -> friends, user);
  return 0;
}

/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 */
int follow_brand(User *user, char *brand_name)
{
  if(get_brand_index(brand_name) == -1){
    return -1;
  }
  if(in_brand_list(user -> brands, brand_name) == true){
    return -1;
  }
  user -> brands = insert_into_brand_list(user -> brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
  //brand_name is invalid
  if(get_brand_index(brand_name) == -1){
    return -1;
  }
  //no link at the beginning
  if(in_brand_list(user -> brands, brand_name) == false){
    return -1;
  }
  user -> brands = delete_from_brand_list(user -> brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Return the number of mutual friends between two users.
 */
int get_mutual_friends(User *a, User *b)
{
  int count = 0;
  for(FriendNode *p1 = a -> friends; p1 != NULL; p1 = p1 -> next){
    if(in_friend_list(b -> friends, p1 -> user)){
      count = count + 1;
    }
  }
  return count;
}

/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 */
int get_degrees_of_connection(User *a, User *b)
{
  //specific case: same
  if(a == b) return 0;
  FriendNode *p = allUsers;
  int total_users = 0, start = 0, end = 1;
  //set all users' visited false and count total users
  while(p != NULL) {
    total_users++;
    p -> user -> visited = false;
    p = p -> next;
  }
  User *queue[total_users];
  //add the first user
  queue[0] = a;
  a -> visited = true;
  a -> distance = 0;
  //begin loop
  while(start != end){
    User *q = queue[start];
    for(FriendNode *ptr = q -> friends; ptr != NULL; ptr = ptr -> next){
      if(ptr -> user -> visited == false) {
        if(ptr -> user == b) {
          return (queue[start] -> distance) + 1;
        }else{
          int last_dist = (queue[start] -> distance);
          ptr -> user -> distance = last_dist + 1;
          ptr -> user -> visited = true;
          queue[end] = ptr -> user;
          end = end + 1;
        }
      }
    }
    start = start + 1;
  }
  return -1;
}

/**
 * TODO: Complete this function
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
  int idx_a = get_brand_index(brandNameA);
  if(idx_a == -1) return;
  int idx_b = get_brand_index(brandNameB);
  if(idx_b == -1) return;
  brand_adjacency_matrix[idx_a][idx_b] = 1;
  brand_adjacency_matrix[idx_b][idx_a] = 1;
}

/**
 * TODO: Complete this function
 * Marks two brands as not similar.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
  int idx_a = get_brand_index(brandNameA);
  if(idx_a == -1) return;
  int idx_b = get_brand_index(brandNameB);
  if(idx_b == -1) return;
  brand_adjacency_matrix[idx_a][idx_b] = 0;
  brand_adjacency_matrix[idx_b][idx_a] = 0;
}

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.
 */
User *get_suggested_friend(User *user)
{
  User *final_user = NULL;
  FriendNode *curr = allUsers;
  int max = -1, mutual_brands;
  while(curr != NULL){
    //The suggested friend must be a valid user, cannot be the user themself
    if(curr -> user == user){curr = curr -> next; continue;}
    //cannot be some that they're already friends with
    if(in_friend_list(curr -> user -> friends, user)){curr = curr -> next; continue;}
    mutual_brands = 0;
    for(BrandNode *p = curr -> user -> brands; p != NULL; p = p -> next){
      if(in_brand_list(user -> brands, p -> brand_name)){
        mutual_brands = mutual_brands + 1;
      }
    }
    if(mutual_brands >= max){
      max = mutual_brands;
      final_user = curr -> user;
    }
    curr = curr -> next;
  }
  return final_user;
}

/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */
int add_suggested_friends(User *user, int n)
{
  int num = 0;
  User *final_user;
  for(int k = 0; k < n; k++){
    if((final_user = get_suggested_friend(user)) == NULL){
      return num;
    }else{
      add_friend(user, final_user);
      num = num + 1;
    }
  }
}

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
char *get_suggested_brand(User *user){
  int similar_count_array[MAT_SIZE], i = 0, x;
  while(i < MAT_SIZE){similar_count_array[i] = 0; i++;}
  for(BrandNode *b_p = user -> brands; b_p != NULL; b_p = b_p -> next){
    x = get_brand_index(b_p -> brand_name);
    for(int y = 0; y < MAT_SIZE; y++){
      //The suggested brand must be a valid brand
      if(in_brand_list(user -> brands, brand_names[y])){
        similar_count_array[y] = -2;
      }
      //cannot be a brand that the user already follows
      else if(brand_adjacency_matrix[x][y] == 1){
        similar_count_array[y] = similar_count_array[y] + 1;
      }
    }
  }
  int max = -1;
  char *final_brand = NULL;
  for(int idx = 0; idx < MAT_SIZE; idx++){
    if(similar_count_array[idx] > max){
      final_brand = brand_names[idx];
      max = similar_count_array[idx];
    }
    else if(similar_count_array[idx] == max){
      if(strcmp(final_brand, brand_names[idx]) < 0){
        final_brand = brand_names[idx];
      }
    }
  }
  return final_brand;
}

int follow_suggested_brands(User *user, int n)
{
  int num = 0;
  char *brand_name;
  for(int i = 0; i < n; i++){
    if((brand_name = get_suggested_brand(user)) == NULL){
      return num;
    }
    follow_brand(user, brand_name);
    num = num + 1;
  }
  return num;
}
