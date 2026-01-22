// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// // 哈希表桶的大小（建议取质数，减少冲突，可根据数据量调整）
// #define HASH_TABLE_SIZE 97  

// // 定义键值对节点（链表结构，解决哈希冲突）
// typedef struct KeyValueNode {
//     char *key;
//     int value;
//     struct KeyValueNode *next;  // 下一个冲突节点
// } KeyValueNode;

// // 定义哈希map：哈希表（桶数组）+ 总元素数
// typedef struct {
//     KeyValueNode *buckets[HASH_TABLE_SIZE];
//     int size;  // 当前总元素数
// } StringIntHashMap;

// // 核心：字符串哈希函数（DJB2算法，分布均匀、效率高）
// static unsigned int hash_func(const char *key) {
//     unsigned int hash = 5381;  // 初始值（经典DJB2参数）
//     int c;
//     while ((c = *key++)) {
//         hash = ((hash << 5) + hash) + c;  // hash = hash*33 + c
//     }
//     return hash % HASH_TABLE_SIZE;  // 映射到桶的索引
// }

// // 1. 初始化哈希map
// void hash_map_init(StringIntHashMap *map) {
//     if (map == NULL) return;
//     map->size = 0;
//     // 初始化所有桶为NULL
//     for (int i = 0; i < HASH_TABLE_SIZE; i++) {
//         map->buckets[i] = NULL;
//     }
// }

// // 辅助函数：创建新节点（内部使用）
// static KeyValueNode *create_node(const char *key, int value) {
//     KeyValueNode *node = (KeyValueNode *)malloc(sizeof(KeyValueNode));
//     if (node == NULL) return NULL;
//     // 分配key内存并拷贝
//     node->key = (char *)malloc(strlen(key) + 1);
//     if (node->key == NULL) {
//         free(node);
//         return NULL;
//     }
//     strcpy(node->key, key);
//     node->value = value;
//     node->next = NULL;
//     return node;
// }

// // 2. 添加/更新键值对（平均O(1)）
// // 返回值：0=成功，1=内存分配失败，2=参数错误
// int hash_map_put(StringIntHashMap *map, const char *key, int value) {
//     if (map == NULL || key == NULL) return 2;

//     // 1. 计算哈希值，定位桶
//     unsigned int bucket_idx = hash_func(key);
//     KeyValueNode *curr = map->buckets[bucket_idx];

//     // 2. 检查桶内是否已有该key，有则更新value
//     while (curr != NULL) {
//         if (strcmp(curr->key, key) == 0) {
//             curr->value = value;
//             return 0;
//         }
//         curr = curr->next;
//     }

//     // 3. 无该key，创建新节点并插入桶的头部（最快插入方式）
//     KeyValueNode *new_node = create_node(key, value);
//     if (new_node == NULL) return 1;
//     new_node->next = map->buckets[bucket_idx];
//     map->buckets[bucket_idx] = new_node;
//     map->size++;
//     return 0;
// }

// // 3. 根据key查找value（平均O(1)）
// // 返回值：0=找到，1=未找到，2=参数错误
// int hash_map_get(StringIntHashMap *map, const char *key, int *value) {
//     if (map == NULL || key == NULL || value == NULL) return 2;

//     // 1. 计算哈希值，定位桶
//     unsigned int bucket_idx = hash_func(key);
//     KeyValueNode *curr = map->buckets[bucket_idx];

//     // 2. 遍历桶内链表查找key
//     while (curr != NULL) {
//         if (strcmp(curr->key, key) == 0) {
//             *value = curr->value;
//             return 0;
//         }
//         curr = curr->next;
//     }
//     return 1;
// }

// // 4. 删除指定key（平均O(1)）
// // 返回值：0=成功，1=未找到，2=参数错误
// int hash_map_remove(StringIntHashMap *map, const char *key) {
//     if (map == NULL || key == NULL) return 2;

//     // 1. 计算哈希值，定位桶
//     unsigned int bucket_idx = hash_func(key);
//     KeyValueNode *curr = map->buckets[bucket_idx];
//     KeyValueNode *prev = NULL;

//     // 2. 遍历桶内链表找目标节点
//     while (curr != NULL) {
//         if (strcmp(curr->key, key) == 0) {
//             // 3. 从链表中移除节点
//             if (prev == NULL) {
//                 // 目标节点是桶的第一个节点
//                 map->buckets[bucket_idx] = curr->next;
//             } else {
//                 prev->next = curr->next;
//             }
//             // 4. 释放节点内存
//             free(curr->key);
//             free(curr);
//             map->size--;
//             return 0;
//         }
//         prev = curr;
//         curr = curr->next;
//     }
//     return 1;
// }

// // 5. 销毁哈希map，释放所有内存
// void hash_map_destroy(StringIntHashMap *map) {
//     if (map == NULL) return;

//     // 遍历所有桶，释放链表节点
//     for (int i = 0; i < HASH_TABLE_SIZE; i++) {
//         KeyValueNode *curr = map->buckets[i];
//         while (curr != NULL) {
//             KeyValueNode *temp = curr;
//             curr = curr->next;
//             // 释放key和节点
//             free(temp->key);
//             free(temp);
//         }
//         map->buckets[i] = NULL;
//     }
//     map->size = 0;
// }

// // 测试示例（对比原数组版，数据量大时速度差异明显）
// int main() {
//     StringIntHashMap my_map;
//     hash_map_init(&my_map);

//     // 1. 添加/更新键值对
//     hash_map_put(&my_map, "apple", 5);
//     hash_map_put(&my_map, "banana", 3);
//     hash_map_put(&my_map, "orange", 8);
//     hash_map_put(&my_map, "apple", 10);  // 更新

//     // 2. 查找
//     int value;
//     if (hash_map_get(&my_map, "apple", &value) == 0) {
//         printf("apple: %d\n", value);  // 输出10
//     }
//     if (hash_map_get(&my_map, "grape", &value) == 1) {
//         printf("grape: 未找到\n");
//     }

//     // 3. 删除
//     hash_map_remove(&my_map, "banana");
//     if (hash_map_get(&my_map, "banana", &value) == 1) {
//         printf("banana: 未找到\n");
//     }

//     // 4. 销毁
//     hash_map_destroy(&my_map);
//     return 0;
// }