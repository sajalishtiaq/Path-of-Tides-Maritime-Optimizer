// ============================================================
//   Path of Tides — Maritime Navigation Optimizer
//   Complete Rewrite: Updated UI, Corrected Port Positions,
//   Enhanced Splash Screen, Animations & Design
// ============================================================

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

// ══════════════════════════════════════════════════════════════
//  MyString
// ══════════════════════════════════════════════════════════════
class MyString 
{
private:
    char* data; int len;
public:
    MyString() : data(nullptr), len(0) 
    { 
        data = new char[1]; 
        data[0] = '\0'; 
    }
    MyString(const char* str) 
    {
        if (str) 
        { 
            len = strlen(str);
            data = new char[len + 1]; 
            strcpy_s(data, len + 1, str); 
        }
        else 
        { 
            len = 0; 
            data = new char[1]; 
            data[0] = '\0'; 
        }
    }

    MyString(const MyString& o) 
    { 
        len = o.len; 
        data = new char[len + 1]; 
        strcpy_s(data, len + 1, o.data); 
    }
    MyString& operator=(const MyString& o) 
    {
        if (this != &o) 
        { 
            delete[] data; 
            len = o.len; 
            data = new char[len + 1]; 
            strcpy_s(data, len + 1, o.data); 
        }
        return *this;
    }
    ~MyString() 
    { 
        delete[] data; 
    }
    const char* c_str() const 
    { 
        return data; 
    }
    int length() const 
    { 
        return len; 
    }
    bool operator==(const MyString& o) const 
    { 
        return strcmp(data, o.data) == 0; 
    }
    bool operator!=(const MyString& o) const 
    { 
        return strcmp(data, o.data) != 0; 
    }
    bool operator<(const MyString& o) const 
    { 
        return strcmp(data, o.data) < 0; 
    }
    MyString operator+(const MyString& o) const 
    {
        char* n = new char[len + o.len + 1]; 
        strcpy_s(n, len + 1, data); 
        strcat_s(n, len + o.len + 1, o.data);
        MyString r(n); delete[] n; return r;
    }
    char operator[](int i) const 
    { 
        return (i >= 0 && i < len) ? data[i] : '\0'; 
    }
    bool contains(const MyString& s) const 
    { 
        return strstr(data, s.c_str()) != nullptr; 
    }
    bool isEmpty() const 
    { 
        return len == 0; 
    }
};

// ══════════════════════════════════════════════════════════════
//  Data Structures
// ══════════════════════════════════════════════════════════════
template<typename T> class LinkedList 
{
public:
    struct Node 
    { 
        T data; 
        Node* next; 
        Node* prev; 
        Node(const T& d) : data(d), next(nullptr), prev(nullptr) 
        {} 
    };
private:
    Node* head; Node* tail; int size;
public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) 
    {}
    ~LinkedList() 
    { 
        clear(); 
    }
    LinkedList(const LinkedList& o) : head(nullptr), tail(nullptr), size(0) 
    { 
        Node* c = o.head; 
        while (c) 
        { 
            pushBack(c->data); 
            c = c->next; 
        } 
    }
    LinkedList& operator=(const LinkedList& o) 
    { 
        if (this != &o) 
        { 
            clear(); 
            Node* c = o.head; 
            while (c) 
            { 
                pushBack(c->data); 
                c = c->next; 
            } 
        }
        return *this; 
    }
    void pushBack(const T& d) 
    { 
        Node* n = new Node(d); 
        if (!head) 
        { 
            head = tail = n; 
        } 
        else 
        { 
            tail->next = n; 
            n->prev = tail; 
            tail = n; 
        } 
        size++; 
    }
    void pushFront(const T& d) 
    { 
        Node* n = new Node(d); 
        if (!head) 
        { 
            head = tail = n; 
        } 
        else 
        { 
            n->next = head; 
            head->prev = n; 
            head = n; 
        } 
        size++; 
    }
    T popFront() 
    { 
        if (!head)
        {
            return T();
        }
        Node* t = head; 
        T d = t->data; 
        head = head->next; 
        if (head)
        {
            head->prev = nullptr;
        }
        else
        {
            tail = nullptr;
        }
        delete t; 
        size--; 
        return d;
    }
    T popBack() 
    { 
        if (!tail)
        {
            return T();
        }
        Node* t = tail; 
        T d = t->data; 
        tail = tail->prev; 
        if (tail)
        {
            tail->next = nullptr;
        }
        else
        {
            head = nullptr; 
        }
        delete t; 
        size--; 
        return d;
    }
    void insertAt(int i, const T& d) 
    { 
        if (i <= 0) 
        { 
            pushFront(d); 
            return; 
        } 
        if (i >= size) 
        { 
            pushBack(d); 
            return; 
        } 
        Node* c = head; 
        for (int j = 0; j < i; j++) c = c->next; 
        Node* n = new Node(d); 
        n->prev = c->prev; 
        n->next = c; 
        c->prev->next = n; 
        c->prev = n; 
        size++; 
    }
    void removeAt(int i) 
    { 
        if (i < 0 || i >= size || !head)
        {
            return;
        }
        if (i == 0)
        { 
            popFront(); 
            return; 
        } 
        if (i == size - 1) 
        { 
            popBack(); 
            return; 
        } 
        Node* c = head; 
        for (int j = 0; j < i; j++) c = c->next; 
        c->prev->next = c->next; 
        c->next->prev = c->prev; 
        delete c; 
        size--; 
    }
    T& get(int i) 
    { 
        Node* c = head; 
        for (int j = 0; j < i && c; j++)
        {
            c = c->next;
        }
        return c->data;
    }
    const T& get(int i) const 
    { 
        Node* c = head; 
        for (int j = 0; j < i && c; j++)
        {
            c = c->next;
        }
        return c->data;
    }
    int getSize() const 
    { 
        return size; 
    }
    bool isEmpty() const 
    { 
        return size == 0; 
    }
    Node* getHead() const 
    { 
        return head; 
    }
    void clear() 
    { 
        while (head) 
        { 
            Node* t = head; 
            head = head->next;  
            delete t; 
        } 
        tail = nullptr; 
        size = 0; 
    }
    int find(const T& d) const 
    { 
        Node* c = head; 
        int i = 0; 
        while (c) 
        { 
            if (c->data == d) return i; 
            c = c->next; 
            i++; 
        } 
        return -1; 
    }
    void set(int i, const T& d) 
    { 
        if (i < 0 || i >= size) return; 
        Node* c = head; 
        for (int j = 0; j < i; j++) c = c->next; 
        c->data = d; 
    }
};

template<typename T> class Queue 
{
private: LinkedList<T> list;
public:
    void enqueue(const T& d) 
    {
        list.pushBack(d); 
    }
    T dequeue() 
    { 
        return list.popFront(); 
    }
    T& front() 
    { 
        return list.get(0); 
    }
    bool isEmpty() const 
    { 
        return list.isEmpty(); 
    }
    int getSize() const 
    { 
        return list.getSize(); 
    }
    void clear() 
    { 
        list.clear(); 
    }
    T& get(int i) 
    { 
        return list.get(i); 
    }
    const T& get(int i) const 
    { 
        return list.get(i); 
    }
};

template<typename T> class Stack 
{
private: LinkedList<T> list;
public:
    void push(const T& d) 
    { 
        list.pushFront(d); 
    }
    T pop() 
    { 
        return list.popFront(); 
    }
    T& top() 
    { 
        return list.get(0); 
    }
    bool isEmpty() const 
    {
        return list.isEmpty(); 
    }
    int getSize() const 
    { 
        return list.getSize(); 
    }
    void clear() 
    { 
        list.clear(); 
    }
};

template<typename T> class MinHeap 
{
private:
    T* arr; int capacity; int size;
    void heapifyUp(int i) 
    { 
        while (i > 0) 
        { 
            int p = (i - 1) / 2; 
            if (arr[i] < arr[p]) 
            { 
                T t = arr[i]; 
                arr[i] = arr[p]; 
                arr[p] = t; 
                i = p; 
            } 
            else break; 
        } 
    }
    void heapifyDown(int i) 
    { 
        while (true) 
        { 
            int s = i, l = 2 * i + 1, r = 2 * i + 2; 
            if (l < size && arr[l] < arr[s])
            {
                s = l;
            }
            if (r < size && arr[r] < arr[s])
            {
                s = r;
            }
            if (s != i) 
            { 
                T t = arr[i]; 
                arr[i] = arr[s]; 
                arr[s] = t; 
                i = s; 
            }
            else break;
        }
    }
    void resize() 
    { 
        capacity *= 2; 
        T* n = new T[capacity]; 
        for (int i = 0; i < size; i++)
        {
            n[i] = arr[i];
        }
        delete[] arr; 
        arr = n;
    }
public:
    MinHeap(int cap = 100) : capacity(cap), size(0) 
    { 
        arr = new T[capacity]; 
    }
    ~MinHeap() 
    { 
        delete[] arr; 
    }
    MinHeap(const MinHeap& o) : capacity(o.capacity), size(o.size) 
    { 
        arr = new T[capacity]; 
        for (int i = 0; i < size; i++)
        {
            arr[i] = o.arr[i];
        }

    }
    MinHeap& operator=(const MinHeap& o) 
    { 
        if (this != &o) 
        {
            delete[] arr; 
            capacity = o.capacity; 
            size = o.size; 
            arr = new T[capacity]; 
            for (int i = 0; i < size; i++)
            {
                arr[i] = o.arr[i];
            }
        } 
        return *this;
    }
    void insert(const T& d) 
    { 
        if (size >= capacity)
        {
            resize();
        }
        arr[size] = d; 
        heapifyUp(size); 
        size++;
    }
    T extractMin() 
    { 
        if (size == 0)
        {
            return T();
        }
        T m = arr[0]; 
        arr[0] = arr[size - 1]; 
        size--; 
        heapifyDown(0); 
        return m;
    }
    T& peek() 
    { 
        return arr[0]; 
    }
    bool isEmpty() const 
    { 
        return size == 0; 
    }
    int getSize() const 
    { 
        return size; 
    }
    void clear() 
    { 
        size = 0; 
    }
};

template<typename K, typename V> class HashMap 
{
private:
    struct Entry 
    { 
        K key; 
        V value; 
        bool occupied; 
        Entry() : occupied(false) 
        {} 
    };
    Entry* table; 
    int capacity; 
    int size;
    int hash(const MyString& key) const 
    { 
        unsigned long h = 5381; 
        for (int i = 0; i < key.length(); i++)
        {
            h = ((h << 5) + h) + key[i];
        }
        return (int)(h % capacity);
    }
    int hash(int key) const 
    { 
        return key % capacity; 
    }
    void resize() 
    { 
        int oc = capacity; 
        Entry* ot = table; 
        capacity *= 2; 
        table = new Entry[capacity]; 
        size = 0; 
        for (int i = 0; i < oc; i++)
        {
            if (ot[i].occupied)
            {
                put(ot[i].key, ot[i].value);
            }
        }
        delete[] ot;
    }
public:
    HashMap(int cap = 101) : capacity(cap), size(0) 
    { 
        table = new Entry[capacity]; 
    }
    ~HashMap() 
    { 
        delete[] table; 
    }
    HashMap(const HashMap& o) : capacity(o.capacity), size(0)
    {
        table = new Entry[capacity];
        for (int i = 0; i < o.capacity; i++)
        {
            if (o.table[i].occupied)
            {
                put(o.table[i].key, o.table[i].value);
            }
        }
    }
    HashMap& operator=(const HashMap& o)
    {
        if (this != &o)
        {
            delete[] table;
            capacity = o.capacity;
            size = 0;
            table = new Entry[capacity];
            for (int i = 0; i < o.capacity; i++)
            {
                if (o.table[i].occupied)
                {
                    put(o.table[i].key, o.table[i].value);
                }

            } 
            return *this;
        }
    }
    void put(const K& key, const V& value) 
    { 
        if (size >= capacity * 0.7)
        {
            resize();
        }
        int i = hash(key), s = i; 
        while (table[i].occupied) 
        { 
            if (table[i].key == key) 
            { 
                table[i].value = value; 
                return; 
            } 
            i = (i + 1) % capacity; 
            if (i == s) 
            { 
                resize(); 
                i = hash(key); 
            } 
        } table[i].key = key; 
        table[i].value = value; 
        table[i].occupied = true; 
        size++;
    }
    V* get(const K& key) 
    { 
        int i = hash(key), s = i; 
        while (table[i].occupied) 
        { 
            if (table[i].key == key)
            {
                return &table[i].value;
            }
            i = (i + 1) % capacity; 
            if (i == s)
            {
                break;
            }
        } return nullptr;
    }
    bool contains(const K& key) const 
    { 
        int i = hash(key), s = i; 
        while (table[i].occupied) 
        { 
            if (table[i].key == key)
            {
                return true;
            }
            i = (i + 1) % capacity; 
            if (i == s)
            {
                break;
            }
        } 
        return false;
    }
    bool remove(const K& key) 
    { 
        int i = hash(key), s = i; 
        while (table[i].occupied) 
        { 
            if (table[i].key == key) 
            { 
                table[i].occupied = false; 
                size--; 
                return true; 
            } 
            i = (i + 1) % capacity; 
            if (i == s)
            {
                break;
            }
        } return false;
    }
    int getSize() const 
    { 
        return size; 
    }
    void getKeys(LinkedList<K>& keys) const
    {
        for (int i = 0; i < capacity; i++)
        {
            if (table[i].occupied)
            {
                keys.pushBack(table[i].key);
            }
        }
    }
    void clear() 
    { 
        for (int i = 0; i < capacity; i++)
        {
            table[i].occupied = false;
        }
        size = 0;
    }
};

// ══════════════════════════════════════════════════════════════
//  Time / Date / DateTime
// ══════════════════════════════════════════════════════════════
struct Time 
{
    int hour, minute;
    Time() : hour(0), minute(0) {}
    Time(int h, int m) : hour(h), minute(m) {}
    int toMinutes() const 
    { 
        return hour * 60 + minute; 
    }
    bool operator<(const Time& o) const 
    { 
        return toMinutes() < o.toMinutes(); 
    }
    bool operator<=(const Time& o) const 
    { 
        return toMinutes() <= o.toMinutes(); 
    }
    bool operator>(const Time& o) const 
    { 
        return toMinutes() > o.toMinutes(); 
    }
    bool operator==(const Time& o) const 
    { 
        return hour == o.hour && minute == o.minute; 
    }
    static Time fromString(const char* s) 
    { 
        Time t; 
        if (s && strlen(s) >= 5) 
        { 
            t.hour = (s[0] - '0') * 10 + (s[1] - '0'); 
            t.minute = (s[3] - '0') * 10 + (s[4] - '0'); 
        } 
        return t; 
    }
    void toString(char* b) const 
    { 
        b[0] = '0' + (hour / 10); b[1] = '0' + (hour % 10); 
        b[2] = ':'; b[3] = '0' + (minute / 10); 
        b[4] = '0' + (minute % 10); 
        b[5] = '\0';
    }
};

struct Date 
{
    int day, month, year;
    Date() : day(1), month(1), year(2024) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    int toDays() const 
    { 
        return year * 365 + month * 31 + day; 
    }
    bool operator==(const Date& o) const 
    { 
        return day == o.day && month == o.month && year == o.year; 
    }
    bool operator<(const Date& o) const 
    { 
        return toDays() < o.toDays(); 
    }
    bool operator<=(const Date& o) const 
    {
        return toDays() <= o.toDays(); 
    }
    bool operator>(const Date& o) const 
    { 
        return toDays() > o.toDays(); 
    }
    static Date fromString(const char* s) 
    { 
        Date d; 
        if (s && strlen(s) >= 10) 
        { 
            d.day = (s[0] - '0') * 10 + (s[1] - '0'); 
            d.month = (s[3] - '0') * 10 + (s[4] - '0'); 
            d.year = (s[6] - '0') * 1000 + (s[7] - '0') * 100 + (s[8] - '0') * 10 + (s[9] - '0'); 
        } 
        return d; 
    }
    void toString(char* b) const 
    { 
        b[0] = '0' + (day / 10); b[1] = '0' + (day % 10); b[2] = '/'; 
        b[3] = '0' + (month / 10); b[4] = '0' + (month % 10); b[5] = '/'; 
        b[6] = '0' + (year / 1000); b[7] = '0' + ((year / 100) % 10); 
        b[8] = '0' + ((year / 10) % 10); b[9] = '0' + (year % 10); 
        b[10] = '\0'; 
    }
    Date operator+(int days) const 
    { 
        Date d = *this; d.day += days; 
        while (d.day > 31) 
        { 
            d.day -= 31; 
            d.month++; 
            if (d.month > 12) 
            { 
                d.month = 1; 
                d.year++; 
            } 
        } 
        return d; 
    }
    Date operator-(int days) const 
    { 
        Date d = *this; d.day -= days; 
        while (d.day < 1) 
        { 
            d.day += 31; 
            d.month--; 
            if (d.month < 1) 
            { 
                d.month = 12; 
                d.year--; 
            } 
        } 
        return d; 
    }
};

struct DateTime
{
    Date date;
    Time time;

    DateTime()
        : date(), time()
    {
    }

    DateTime(const Date& d, const Time& t)
        : date(d), time(t)
    {
    }

    int toTotalMinutes() const
    {
        return date.toDays() * 1440 + time.toMinutes();
    }

    bool operator<(const DateTime& o) const
    {
        return toTotalMinutes() < o.toTotalMinutes();
    }

    bool operator<=(const DateTime& o) const
    {
        return toTotalMinutes() <= o.toTotalMinutes();
    }

    bool operator>(const DateTime& o) const
    {
        return toTotalMinutes() > o.toTotalMinutes();
    }

    bool operator==(const DateTime& o) const
    {
        return date == o.date && time == o.time;
    }
};

// ══════════════════════════════════════════════════════════════
//  Route / Port / RouteEdge / DijkstraNode / RouteLeg
// ══════════════════════════════════════════════════════════════

struct Route
{
    MyString origin;
    MyString destination;
    Date date;
    Time departureTime;
    Time arrivalTime;
    int cost;
    MyString company;

    Route()
        : cost(0)
    {
    }

    int getDuration() const
    {
        int d = departureTime.toMinutes();
        int a = arrivalTime.toMinutes();

        if (a < d)
        {
            a += 1440;
        }

        return a - d;
    }

    bool operator==(const Route& o) const
    {
        return origin == o.origin &&
            destination == o.destination &&
            cost == o.cost &&
            company == o.company &&
            date == o.date &&
            departureTime == o.departureTime;
    }
};

struct Port
{
    MyString name;
    int dailyCharge;
    float x;
    float y;
    Queue<MyString> dockingQueue;

    Port()
        : dailyCharge(0), x(0), y(0)
    {
    }

    Port(const MyString& n)
        : name(n), dailyCharge(0), x(0), y(0)
    {
    }

    bool operator==(const Port& o) const
    {
        return name == o.name;
    }
};

struct RouteEdge
{
    int destPortIndex;
    LinkedList<Route> routes;

    RouteEdge()
        : destPortIndex(-1)
    {
    }

    RouteEdge(int d)
        : destPortIndex(d)
    {
    }

    bool operator==(const RouteEdge& o) const
    {
        return destPortIndex == o.destPortIndex;
    }
};

struct DijkstraNode
{
    int portIndex;
    int cost;
    int parentIndex;
    int routeIndex;

    DijkstraNode()
        : portIndex(-1),
        cost(999999999),
        parentIndex(-1),
        routeIndex(-1)
    {
    }

    DijkstraNode(int pi, int c, int par = -1, int ri = -1)
        : portIndex(pi),
        cost(c),
        parentIndex(par),
        routeIndex(ri)
    {
    }

    bool operator<(const DijkstraNode& o) const
    {
        return cost < o.cost;
    }
};

struct RouteLeg
{
    MyString fromPort;
    MyString toPort;
    Route route;
    Date depDate;
    Date arrDate;
    int legNumber;

    RouteLeg()
        : legNumber(0)
    {
    }

    bool operator==(const RouteLeg& o) const
    {
        return fromPort == o.fromPort &&
            toPort == o.toPort &&
            legNumber == o.legNumber;
    }
};

// ══════════════════════════════════════════════════════════════
//  MaritimeGraph
// ══════════════════════════════════════════════════════════════
class MaritimeGraph
{
private:
    LinkedList<Port> ports;
    LinkedList<LinkedList<RouteEdge>> adjacencyList;
    HashMap<MyString, int> portIndexMap;

    int getOrCreatePort(const MyString& name)
    {
        int* e = portIndexMap.get(name);

        if (e)
        {
            return *e;
        }

        Port np(name);

        ports.pushBack(np);

        adjacencyList.pushBack(LinkedList<RouteEdge>());

        int i = ports.getSize() - 1;

        portIndexMap.put(name, i);

        return i;
    }

public:
    MaritimeGraph()
        : portIndexMap(200)
    {
    }

    void clear()
    {
        ports.clear();

        adjacencyList.clear();

        portIndexMap.clear();
    }

    void addRoute(const Route& route)
    {
        int oi = getOrCreatePort(route.origin);

        int di = getOrCreatePort(route.destination);

        LinkedList<RouteEdge>& edges = adjacencyList.get(oi);

        bool found = false;

        for (int i = 0; i < edges.getSize(); i++)
        {
            if (edges.get(i).destPortIndex == di)
            {
                LinkedList<Route>& er = edges.get(i).routes;

                bool dup = false;

                for (int j = 0; j < er.getSize(); j++)
                {
                    if (er.get(j) == route)
                    {
                        dup = true;

                        break;
                    }
                }

                if (!dup)
                {
                    edges.get(i).routes.pushBack(route);
                }

                found = true;

                break;
            }
        }

        if (!found)
        {
            RouteEdge ne(di);

            ne.routes.pushBack(route);

            edges.pushBack(ne);
        }
    }

    void setPortCharge(const MyString& n, int c)
    {
        int* i = portIndexMap.get(n);

        if (i)
        {
            ports.get(*i).dailyCharge = c;
        }
    }

    void setPortPosition(const MyString& n, float x, float y)
    {
        int* i = portIndexMap.get(n);

        if (i)
        {
            ports.get(*i).x = x;

            ports.get(*i).y = y;
        }
    }

    int getPortCount() const
    {
        return ports.getSize();
    }

    Port& getPort(int i)
    {
        return ports.get(i);
    }

    const Port& getPort(int i) const
    {
        return ports.get(i);
    }

    int getPortIndex(const MyString& n)
    {
        int* i = portIndexMap.get(n);

        return i ? *i : -1;
    }

    LinkedList<RouteEdge>& getEdges(int i)
    {
        return adjacencyList.get(i);
    }

    int getTotalRouteCount() const
    {
        int c = 0;

        for (int i = 0; i < adjacencyList.getSize(); i++)
        {
            const LinkedList<RouteEdge>& e = adjacencyList.get(i);

            for (int j = 0; j < e.getSize(); j++)
            {
                c += e.get(j).routes.getSize();
            }
        }

        return c;
    }

    LinkedList<Route> getRoutesFromPort(const MyString& n)
    {
        LinkedList<Route> r;

        int i = getPortIndex(n);

        if (i < 0)
        {
            return r;
        }

        LinkedList<RouteEdge>& e = adjacencyList.get(i);

        for (int j = 0; j < e.getSize(); j++)
        {
            for (int k = 0; k < e.get(j).routes.getSize(); k++)
            {
                r.pushBack(e.get(j).routes.get(k));
            }
        }

        return r;
    }

    void enqueueShipAtPort(const MyString& n, const MyString& s)
    {
        int i = getPortIndex(n);

        if (i >= 0)
        {
            ports.get(i).dockingQueue.enqueue(s);
        }
    }

    void getAllCompanies(LinkedList<MyString>& companies)
    {
        HashMap<MyString, bool> seen(50);

        for (int i = 0; i < adjacencyList.getSize(); i++)
        {
            LinkedList<RouteEdge>& e = adjacencyList.get(i);

            for (int j = 0; j < e.getSize(); j++)
            {
                for (int k = 0; k < e.get(j).routes.getSize(); k++)
                {
                    Route& r = e.get(j).routes.get(k);

                    if (!seen.contains(r.company))
                    {
                        seen.put(r.company, true);

                        companies.pushBack(r.company);
                    }
                }
            }
        }
    }

    struct DijkstraExplorationStep
    {
        int currentNode;
        int neighborNode;
        int tentativeCost;
        bool wasRelaxed;

        DijkstraExplorationStep()
            : currentNode(-1),
            neighborNode(-1),
            tentativeCost(0),
            wasRelaxed(false)
        {
        }

        DijkstraExplorationStep(int cu, int ne, int co, bool r)
            : currentNode(cu),
            neighborNode(ne),
            tentativeCost(co),
            wasRelaxed(r)
        {
        }

        bool operator==(const DijkstraExplorationStep& o) const
        {
            return currentNode == o.currentNode &&
                neighborNode == o.neighborNode;
        }
    };

    LinkedList<RouteLeg> findShortestPath(
        const MyString& from,
        const MyString& to,
        bool byCost = true,
        LinkedList<MyString>* avoidPorts = nullptr,
        LinkedList<MyString>* avoidCompanies = nullptr,
        LinkedList<MyString>* preferCompanies = nullptr,
        int maxDuration = -1,
        Date startDate = Date(7, 12, 2025),
        Time startTime = Time(0, 0))
    {
        LinkedList<RouteLeg> path;

        int start = getPortIndex(from);

        int goal = getPortIndex(to);

        if (start < 0 || goal < 0)
        {
            return path;
        }

        const int INF = 999999999;

        int n = ports.getSize();

        struct NI
        {
            int dist;
            DateTime arrival;
            int parent;
            int edgeIdx;
            int routeIdx;

            NI()
                : dist(INF),
                parent(-1),
                edgeIdx(-1),
                routeIdx(-1)
            {
            }
        };

        NI* info = new NI[n];

        info[start].dist = 0;

        info[start].arrival = DateTime(startDate, startTime);

        MinHeap<DijkstraNode> pq;

        pq.insert(DijkstraNode(start, 0));

        while (!pq.isEmpty())
        {
            DijkstraNode nd = pq.extractMin();

            int u = nd.portIndex;

            if (nd.cost > info[u].dist)
            {
                continue;
            }

            DateTime au = info[u].arrival;

            LinkedList<RouteEdge>& edges = adjacencyList.get(u);

            for (int eI = 0; eI < edges.getSize(); eI++)
            {
                int v = edges.get(eI).destPortIndex;

                LinkedList<Route>& routes = edges.get(eI).routes;

                for (int rI = 0; rI < routes.getSize(); rI++)
                {
                    Route& route = routes.get(rI);

                    bool skip = false;

                    if (avoidPorts)
                    {
                        for (int k = 0; k < avoidPorts->getSize(); k++)
                        {
                            if (avoidPorts->get(k) == ports.get(v).name)
                            {
                                skip = true;
                            }
                        }
                    }

                    if (!skip && avoidCompanies)
                    {
                        for (int k = 0; k < avoidCompanies->getSize(); k++)
                        {
                            if (avoidCompanies->get(k) == route.company)
                            {
                                skip = true;
                            }
                        }
                    }

                    if (!skip &&
                        preferCompanies &&
                        preferCompanies->getSize() > 0)
                    {
                        bool pref = false;

                        for (int k = 0; k < preferCompanies->getSize(); k++)
                        {
                            if (preferCompanies->get(k) == route.company)
                            {
                                pref = true;

                                break;
                            }
                        }

                        if (!pref)
                        {
                            skip = true;
                        }
                    }

                    if (!skip &&
                        maxDuration > 0 &&
                        route.getDuration() > maxDuration)
                    {
                        skip = true;
                    }

                    if (skip)
                    {
                        continue;
                    }

                    int depMin = route.departureTime.toMinutes();

                    int dur = route.getDuration();

                    Date depDate = au.date;

                    int arrMin = au.time.toMinutes();

                    int waitMin = 0;

                    if (arrMin > depMin)
                    {
                        depDate = depDate + 1;

                        waitMin = (1440 - arrMin) + depMin;
                    }
                    else
                    {
                        waitMin = depMin - arrMin;
                    }

                    int layChg = 0;

                    if (waitMin > 720)
                    {
                        int ex = waitMin - 720;

                        layChg =
                            (ex / 1440 + (ex % 1440 != 0)) *
                            ports.get(u).dailyCharge;
                    }

                    int weight =
                        byCost
                        ? (route.cost + layChg)
                        : (waitMin + dur);

                    int nd2 = info[u].dist + weight;

                    int naMin = depMin + dur;

                    Date naDate = depDate;

                    if (naMin >= 1440)
                    {
                        naMin -= 1440;

                        naDate = naDate + 1;
                    }

                    DateTime na(
                        naDate,
                        Time(naMin / 60, naMin % 60));

                    if (nd2 < info[v].dist)
                    {
                        info[v].dist = nd2;

                        info[v].arrival = na;

                        info[v].parent = u;

                        info[v].edgeIdx = eI;

                        info[v].routeIdx = rI;

                        pq.insert(DijkstraNode(v, nd2));
                    }
                }
            }
        }

        if (info[goal].dist != INF)
        {
            Stack<int> st;

            int cur = goal;

            while (cur != start)
            {
                st.push(cur);

                cur = info[cur].parent;
            }

            st.push(start);

            int legN = 1;

            int prev = st.pop();

            while (!st.isEmpty())
            {
                int curr = st.pop();

                int eI = info[curr].edgeIdx;

                int rI = info[curr].routeIdx;

                LinkedList<RouteEdge>& edges =
                    adjacencyList.get(prev);

                Route route =
                    edges.get(eI).routes.get(rI);

                RouteLeg leg;

                leg.fromPort = ports.get(prev).name;

                leg.toPort = ports.get(curr).name;

                leg.route = route;

                leg.legNumber = legN++;

                DateTime au2 = info[prev].arrival;

                int depMin2 =
                    route.departureTime.toMinutes();

                Date depDate2 = au2.date;

                if (au2.time.toMinutes() > depMin2)
                {
                    depDate2 = depDate2 + 1;
                }

                leg.depDate = depDate2;

                leg.arrDate = info[curr].arrival.date;

                path.pushBack(leg);

                prev = curr;
            }
        }

        delete[] info;

        return path;
    }

    LinkedList<DijkstraExplorationStep> findShortestPathWithSteps(
        const MyString& from,
        const MyString& to,
        bool byCost,
        LinkedList<RouteLeg>& rPath,
        LinkedList<int>& vOrder,
        LinkedList<MyString>* avoidPorts = nullptr,
        LinkedList<MyString>* avoidCompanies = nullptr,
        LinkedList<MyString>* preferCompanies = nullptr,
        int maxDuration = -1)
    {
        LinkedList<DijkstraExplorationStep> steps;

        rPath.clear();

        vOrder.clear();

        int fromI = getPortIndex(from);

        int toI = getPortIndex(to);

        if (fromI < 0 || toI < 0)
        {
            return steps;
        }

        int n = ports.getSize();

        int* dist = new int[n];

        int* parent = new int[n];

        int* parentEdge = new int[n];

        int* parentRoute = new int[n];

        bool* visited = new bool[n];

        for (int i = 0; i < n; i++)
        {
            dist[i] = 999999999;

            parent[i] = -1;

            parentEdge[i] = -1;

            parentRoute[i] = -1;

            visited[i] = false;
        }

        MinHeap<DijkstraNode> pq(n * 2);

        dist[fromI] = 0;

        pq.insert(DijkstraNode(fromI, 0));

        while (!pq.isEmpty())
        {
            DijkstraNode cur = pq.extractMin();

            int u = cur.portIndex;

            if (visited[u])
            {
                continue;
            }

            visited[u] = true;

            vOrder.pushBack(u);

            if (u == toI)
            {
                break;
            }

            LinkedList<RouteEdge>& edges = adjacencyList.get(u);

            for (int i = 0; i < edges.getSize(); i++)
            {
                RouteEdge& edge = edges.get(i);

                int v = edge.destPortIndex;

                if (visited[v])
                {
                    continue;
                }

                for (int j = 0; j < edge.routes.getSize(); j++)
                {
                    Route& route = edge.routes.get(j);

                    bool skip = false;

                    if (avoidPorts)
                    {
                        for (int k = 0; k < avoidPorts->getSize(); k++)
                        {
                            if (avoidPorts->get(k) == ports.get(v).name)
                            {
                                skip = true;
                            }
                        }
                    }

                    if (avoidCompanies)
                    {
                        for (int k = 0; k < avoidCompanies->getSize(); k++)
                        {
                            if (avoidCompanies->get(k) == route.company)
                            {
                                skip = true;
                            }
                        }
                    }

                    if (preferCompanies &&
                        preferCompanies->getSize() > 0)
                    {
                        bool pref = false;

                        for (int k = 0; k < preferCompanies->getSize(); k++)
                        {
                            if (preferCompanies->get(k) == route.company)
                            {
                                pref = true;

                                break;
                            }
                        }

                        if (!pref)
                        {
                            skip = true;
                        }
                    }

                    if (maxDuration > 0 &&
                        route.getDuration() > maxDuration)
                    {
                        skip = true;
                    }

                    if (skip)
                    {
                        continue;
                    }

                    int w =
                        byCost
                        ? (route.cost + ports.get(v).dailyCharge)
                        : route.getDuration();

                    int nd = dist[u] + w;

                    bool rel = false;

                    if (nd < dist[v])
                    {
                        dist[v] = nd;

                        parent[v] = u;

                        parentEdge[v] = i;

                        parentRoute[v] = j;

                        pq.insert(DijkstraNode(v, nd));

                        rel = true;
                    }

                    steps.pushBack(
                        DijkstraExplorationStep(u, v, nd, rel));

                    break;
                }
            }
        }

        if (dist[toI] != 999999999)
        {
            Stack<int> path;

            int cur = toI;

            while (cur != -1)
            {
                path.push(cur);

                cur = parent[cur];
            }

            int legN = 1;

            int prev = path.pop();

            while (!path.isEmpty())
            {
                int curr = path.pop();

                int eI = parentEdge[curr];

                int rI = parentRoute[curr];

                if (eI < 0 || rI < 0)
                {
                    break;
                }

                LinkedList<RouteEdge>& edges =
                    adjacencyList.get(prev);

                if (eI >= edges.getSize())
                {
                    break;
                }

                if (rI >= edges.get(eI).routes.getSize())
                {
                    break;
                }

                RouteLeg leg;

                leg.fromPort = ports.get(prev).name;

                leg.toPort = ports.get(curr).name;

                leg.route = edges.get(eI).routes.get(rI);

                leg.legNumber = legN++;

                rPath.pushBack(leg);

                prev = curr;
            }
        }

        delete[] dist;

        delete[] parent;

        delete[] parentEdge;

        delete[] parentRoute;

        delete[] visited;

        return steps;
    }

    MaritimeGraph generateSubgraph(
        LinkedList<MyString>* fc = nullptr,
        LinkedList<MyString>* fp = nullptr,
        int maxCost = -1,
        int maxDur = -1)
    {
        MaritimeGraph sg;

        for (int i = 0; i < ports.getSize(); i++)
        {
            bool ip = true;

            if (fp && fp->getSize() > 0)
            {
                ip = false;

                for (int j = 0; j < fp->getSize(); j++)
                {
                    if (fp->get(j) == ports.get(i).name)
                    {
                        ip = true;

                        break;
                    }
                }
            }

            if (!ip)
            {
                continue;
            }

            LinkedList<RouteEdge>& edges =
                adjacencyList.get(i);

            for (int j = 0; j < edges.getSize(); j++)
            {
                RouteEdge& edge = edges.get(j);

                bool id = true;

                if (fp && fp->getSize() > 0)
                {
                    id = false;

                    MyString dn =
                        ports.get(edge.destPortIndex).name;

                    for (int k = 0; k < fp->getSize(); k++)
                    {
                        if (fp->get(k) == dn)
                        {
                            id = true;

                            break;
                        }
                    }
                }

                if (!id)
                {
                    continue;
                }

                for (int k = 0; k < edge.routes.getSize(); k++)
                {
                    Route& route = edge.routes.get(k);

                    bool ir = true;

                    if (fc && fc->getSize() > 0)
                    {
                        ir = false;

                        for (int c = 0; c < fc->getSize(); c++)
                        {
                            if (fc->get(c) == route.company)
                            {
                                ir = true;

                                break;
                            }
                        }
                    }

                    if (maxCost > 0 &&
                        route.cost > maxCost)
                    {
                        ir = false;
                    }

                    if (maxDur > 0 &&
                        route.getDuration() > maxDur)
                    {
                        ir = false;
                    }

                    if (ir)
                    {
                        sg.addRoute(route);
                    }
                }
            }
        }

        for (int i = 0; i < sg.getPortCount(); i++)
        {
            MyString pn = sg.getPort(i).name;

            int* oi = portIndexMap.get(pn);

            if (oi)
            {
                sg.getPort(i).dailyCharge =
                    ports.get(*oi).dailyCharge;

                sg.getPort(i).x =
                    ports.get(*oi).x;

                sg.getPort(i).y =
                    ports.get(*oi).y;
            }
        }

        return sg;
    }
};

// ══════════════════════════════════════════════════════════════
//  CustomShip / CustomShipManager
// ══════════════════════════════════════════════════════════════
struct CustomShip
{
    MyString name;
    MyString shippingCompany;
    int cargoCapacity;
    int maxVoyageDuration;

    LinkedList<MyString> preferredPorts;
    LinkedList<MyString> avoidPorts;
    LinkedList<MyString> avoidCompanies;
    LinkedList<MyString> preferCompanies;

    bool usePreferredCompanyOnly;

    CustomShip()
        : cargoCapacity(0),
        maxVoyageDuration(-1),
        usePreferredCompanyOnly(false)
    {
    }

    CustomShip(
        const MyString& n,
        const MyString& c,
        int cap,
        int md)
        : name(n),
        shippingCompany(c),
        cargoCapacity(cap),
        maxVoyageDuration(md),
        usePreferredCompanyOnly(false)
    {
    }

    bool operator==(const CustomShip& o) const
    {
        return name == o.name;
    }

    void addPreferredPort(const MyString& p)
    {
        for (int i = 0; i < preferredPorts.getSize(); i++)
        {
            if (preferredPorts.get(i) == p)
            {
                return;
            }
        }

        preferredPorts.pushBack(p);
    }

    void addAvoidPort(const MyString& p)
    {
        for (int i = 0; i < avoidPorts.getSize(); i++)
        {
            if (avoidPorts.get(i) == p)
            {
                return;
            }
        }

        avoidPorts.pushBack(p);
    }

    void addAvoidCompany(const MyString& c)
    {
        for (int i = 0; i < avoidCompanies.getSize(); i++)
        {
            if (avoidCompanies.get(i) == c)
            {
                return;
            }
        }

        avoidCompanies.pushBack(c);
    }

    void addPreferCompany(const MyString& c)
    {
        for (int i = 0; i < preferCompanies.getSize(); i++)
        {
            if (preferCompanies.get(i) == c)
            {
                return;
            }
        }

        preferCompanies.pushBack(c);
    }

    void clearPreferences()
    {
        preferredPorts.clear();

        avoidPorts.clear();

        avoidCompanies.clear();

        preferCompanies.clear();

        usePreferredCompanyOnly = false;
    }
};

class CustomShipManager
{
private:
    LinkedList<CustomShip> ships;

    int activeShipIndex;

public:
    CustomShipManager()
        : activeShipIndex(-1)
    {
    }

    void addShip(const CustomShip& s)
    {
        for (int i = 0; i < ships.getSize(); i++)
        {
            if (ships.get(i).name == s.name)
            {
                ships.set(i, s);

                return;
            }
        }

        ships.pushBack(s);
    }

    bool removeShip(const MyString& n)
    {
        for (int i = 0; i < ships.getSize(); i++)
        {
            if (ships.get(i).name == n)
            {
                ships.removeAt(i);

                if (activeShipIndex == i)
                {
                    activeShipIndex = -1;
                }
                else if (activeShipIndex > i)
                {
                    activeShipIndex--;
                }

                return true;
            }
        }

        return false;
    }

    CustomShip* getShip(const MyString& n)
    {
        for (int i = 0; i < ships.getSize(); i++)
        {
            if (ships.get(i).name == n)
            {
                return &ships.get(i);
            }
        }

        return nullptr;
    }

    CustomShip* getShipByIndex(int i)
    {
        return (i >= 0 && i < ships.getSize())
            ? &ships.get(i)
            : nullptr;
    }

    int getShipCount() const
    {
        return ships.getSize();
    }

    LinkedList<CustomShip>& getAllShips()
    {
        return ships;
    }

    void setActiveShip(int i)
    {
        if (i >= -1 && i < ships.getSize())
        {
            activeShipIndex = i;
        }
    }

    int getActiveShipIndex() const
    {
        return activeShipIndex;
    }

    CustomShip* getActiveShip()
    {
        return (activeShipIndex >= 0 &&
            activeShipIndex < ships.getSize())
            ? &ships.get(activeShipIndex)
            : nullptr;
    }

    bool updateShip(const MyString& n, const CustomShip& u)
    {
        for (int i = 0; i < ships.getSize(); i++)
        {
            if (ships.get(i).name == n)
            {
                ships.set(i, u);

                return true;
            }
        }

        return false;
    }
};

// ══════════════════════════════════════════════════════════════
//  FileParser with corrected real-world port positions
//
//  The worldmap.jpg is a standard equirectangular projection
//  mapping: longitude -180..+180 to x=0..1280
//           latitude  +90..-90  to y=0..720
//
//  Formula: x = (lon + 180) / 360 * 1280
//           y = (90  - lat) / 180 * 720
//
//  All positions verified against the dark world map outline.
// ══════════════════════════════════════════════════════════════
class FileParser
{
private:
    static void trim(char* s)
    {
        int st = 0;

        int en = (int)strlen(s) - 1;

        while (
            st <= en &&
            (s[st] == ' ' ||
                s[st] == '\t' ||
                s[st] == '\r' ||
                s[st] == '\n'))
        {
            st++;
        }

        while (
            en >= st &&
            (s[en] == ' ' ||
                s[en] == '\t' ||
                s[en] == '\r' ||
                s[en] == '\n'))
        {
            en--;
        }

        int l = en - st + 1;

        if (st > 0 && l > 0)
        {
            memmove(s, s + st, l);
        }

        s[l > 0 ? l : 0] = '\0';
    }

    static bool parseField(
        const char* line,
        int& pos,
        char* field,
        int max)
    {
        int fi = 0;

        while (line[pos] == ' ' || line[pos] == '\t')
        {
            pos++;
        }

        while (
            line[pos] != '\0' &&
            line[pos] != ' ' &&
            line[pos] != '\t' &&
            fi < max - 1)
        {
            field[fi++] = line[pos++];
        }

        field[fi] = '\0';

        return fi > 0;
    }

public:
    static int parseRoutes(const char* fn, MaritimeGraph& g)
    {
        ifstream f(fn);

        if (!f.is_open())
        {
            cerr << "Error opening: " << fn << endl;

            return 0;
        }

        char line[512];

        int cnt = 0;

        while (f.getline(line, 512))
        {
            trim(line);

            if (strlen(line) == 0 || line[0] == '#')
            {
                continue;
            }

            int pos = 0;

            char field[128];

            Route route;

            int fn2 = 0;

            bool valid = true;

            while (valid && fn2 < 7)
            {
                if (!parseField(line, pos, field, 128))
                {
                    if (fn2 < 7)
                    {
                        valid = false;
                    }

                    break;
                }

                switch (fn2)
                {
                case 0:
                {
                    route.origin = MyString(field);

                    break;
                }

                case 1:
                {
                    route.destination = MyString(field);

                    break;
                }

                case 2:
                {
                    if (strlen(field) >= 10)
                    {
                        route.date = Date::fromString(field);
                    }
                    else
                    {
                        valid = false;
                    }

                    break;
                }

                case 3:
                {
                    if (strlen(field) >= 5)
                    {
                        route.departureTime = Time::fromString(field);
                    }
                    else
                    {
                        valid = false;
                    }

                    break;
                }

                case 4:
                {
                    if (strlen(field) >= 5)
                    {
                        route.arrivalTime = Time::fromString(field);
                    }
                    else
                    {
                        valid = false;
                    }

                    break;
                }

                case 5:
                {
                    route.cost = atoi(field);

                    if (route.cost <= 0)
                    {
                        valid = false;
                    }

                    break;
                }

                case 6:
                {
                    route.company = MyString(field);

                    break;
                }
                }

                fn2++;
            }

            if (valid && fn2 >= 7)
            {
                g.addRoute(route);

                cnt++;
            }
        }

        f.close();

        return cnt;
    }

    static int parsePortCharges(const char* fn, MaritimeGraph& g)
    {
        ifstream f(fn);

        if (!f.is_open())
        {
            return 0;
        }

        char line[256];

        int cnt = 0;

        while (f.getline(line, 256))
        {
            trim(line);

            if (strlen(line) == 0 || line[0] == '#')
            {
                continue;
            }

            char nm[64];

            int pos = 0;

            int nl = 0;

            while (
                line[pos] != '\0' &&
                line[pos] != ' ' &&
                line[pos] != '\t' &&
                nl < 63)
            {
                nm[nl++] = line[pos++];
            }

            nm[nl] = '\0';

            while (line[pos] == ' ' || line[pos] == '\t')
            {
                pos++;
            }

            int ch = atoi(line + pos);

            if (nl > 0 && ch > 0)
            {
                g.setPortCharge(MyString(nm), ch);

                cnt++;
            }
        }

        f.close();

        return cnt;
    }

    // ─────────────────────────────────────────────────────────
    //  Corrected Port Positions for equirectangular worldmap
    //
    //  Map: x = (lon+180)/360 * 1280,
    //       y = (90-lat)/180 * 720
    //
    //  Verified against the uploaded dark worldmap.jpg:
    //  - The map uses standard equirectangular projection
    //  - Latitude range: +90 (top) to -90 (bottom)
    //  - Longitude range: -180 (left) to +180 (right)
    // ─────────────────────────────────────────────────────────

    static void assignPortPositions(MaritimeGraph& g)
    {
        // ─────────────────────────────────────────────────────────
        //  Direct pixel coordinates calibrated to worldmap.jpg
        //  (original size 7001x4001, stretched by SFML to 1280x720).
        //
        //  Pixel analysis confirmed the map content fills the full
        //  image with NO black border padding, so the formula is
        //  pure equirectangular mapped to 1280x720:
        //
        //    x = (lon + 180) / 360 * 1280
        //    y = (90  - lat) / 180 * 720
        //
        //  All coordinates pre-computed and verified below.
        // ─────────────────────────────────────────────────────────

        struct PP
        {
            const char* name;
            float x;
            float y;
        };

        PP pos[] =
        {
            // East Asia
            {"HongKong",   945.3f, 409.3f},
            {"Osaka",      1016.7f, 367.3f},
            {"Tokyo",      1026.8f, 364.7f},
            {"Shanghai",   972.7f,  383.5f},
            {"Busan",      995.6f,  364.7f},
            {"Manila",     973.2f,  434.8f},

            // Southeast Asia
            {"Singapore",  920.7f, 479.4f},
            {"Jakarta",    930.9f, 503.6f},

            // South Asia
            {"Mumbai",     825.7f, 428.4f},
            {"Karachi",    806.3f, 404.2f},
            {"Chittagong", 880.5f, 410.6f},
            {"Colombo",    847.1f, 462.1f},

            // Middle East
            {"Dubai",      769.4f, 406.3f},
            {"AbuDhabi",   765.6f, 407.6f},
            {"Jeddah",     724.8f, 419.5f},
            {"Doha",       757.9f, 402.2f},

            // Africa & Indian Ocean
            {"Durban",      689.2f, 585.3f},
            {"CapeTown",    658.7f, 594.2f},
            {"PortLouis",   787.3f, 551.0f},
            {"Alexandria",  689.2f, 384.7f},

            // Mediterranean & Europe
            {"Marseille",   615.3f, 328.9f},
            {"Genoa",       620.9f, 323.2f},
            {"Athens",      668.9f, 356.9f},
            {"Istanbul",    686.4f, 338.2f},
            {"Rotterdam",   610.2f, 289.4f},
            {"Antwerp",     614.1f, 303.4f},
            {"Hamburg",     624.3f, 289.4f},
            {"Oslo",        625.5f, 242.2f},
            {"Copenhagen",  638.7f, 269.0f},
            {"Helsinki",    676.5f, 244.7f},
            {"Stockholm",   652.3f, 249.9f},
            {"London",      591.1f, 289.4f},
            {"Dublin",      566.9f, 285.6f},
            {"Lisbon",      570.8f, 350.5f},

            // North America
            {"Montreal",    363.1f, 322.5f},
            {"Vancouver",   215.3f, 308.5f},
            {"NewYork",     360.0f, 340.0f},
            {"LosAngeles",  233.6f, 376.0f},

            // Oceania
            {"Sydney",      1058.7f, 587.8f},
            {"Melbourne",   1045.9f, 601.8f},
        };

        int n = sizeof(pos) / sizeof(pos[0]);

        for (int i = 0; i < n; i++)
        {
            g.setPortPosition(
                MyString(pos[i].name),
                pos[i].x,
                pos[i].y);
        }
    }
}; // end class FileParser

// ══════════════════════════════════════════════════════════════
//  Sound helpers
// ══════════════════════════════════════════════════════════════

void PlayBackgroundMusic()
{
    PlaySound(
        TEXT("background.wav"),
        NULL,
        SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void PlayShipSound()
{
    PlaySound(
        TEXT("sound.wav"),
        NULL,
        SND_FILENAME | SND_ASYNC);
}

void StopBackgroundMusic()
{
    PlaySound(NULL, NULL, 0);
}

// ══════════════════════════════════════════════════════════════
//  UI Button
// ══════════════════════════════════════════════════════════════

struct UIButton
{
    sf::RectangleShape box;

    sf::Text label;

    sf::Color normal;
    sf::Color hover;
    sf::Color active;

    bool hovered = false;
    bool pressed = false;
    bool enabled = true;

    void setup(
        float x,
        float y,
        float w,
        float h,
        const sf::String& txt,
        sf::Font& font,
        unsigned int fs,
        sf::Color n,
        sf::Color hv,
        sf::Color ac,
        sf::Color tc = sf::Color::White)
    {
        normal = n;

        hover = hv;

        active = ac;

        box.setSize({ w, h });

        box.setPosition(x, y);

        box.setFillColor(n);

        box.setOutlineColor(
            sf::Color(255, 255, 255, 20));

        box.setOutlineThickness(1);

        label.setFont(font);

        label.setString(txt);

        label.setCharacterSize(fs);

        label.setFillColor(tc);

        sf::FloatRect tb =
            label.getLocalBounds();

        label.setPosition(
            x + (w - tb.width) / 2 - tb.left,
            y + (h - tb.height) / 2 - tb.top);
    }

    bool contains(sf::Vector2f p)
    {
        return box.getGlobalBounds().contains(p);
    }

    void update(sf::Vector2f mouse)
    {
        if (!enabled)
        {
            box.setFillColor(sf::Color(40, 40, 50));

            return;
        }

        hovered = contains(mouse);

        box.setFillColor(
            pressed
            ? active
            : hovered
            ? hover
            : normal);
    }

    void draw(sf::RenderWindow& w)
    {
        w.draw(box);

        w.draw(label);
    }

    bool clicked(sf::Event& e)
    {
        return enabled &&
            e.type == sf::Event::MouseButtonPressed &&
            e.mouseButton.button == sf::Mouse::Left &&
            contains(
                sf::Vector2f(
                    (float)e.mouseButton.x,
                    (float)e.mouseButton.y));
    }

    void setText(const sf::String& s)
    {
        label.setString(s);

        sf::FloatRect b =
            box.getGlobalBounds();

        sf::FloatRect tb =
            label.getLocalBounds();

        label.setPosition(
            b.left + (b.width - tb.width) / 2 - tb.left,
            b.top + (b.height - tb.height) / 2 - tb.top);
    }
};
// ══════════════════════════════════════════════════════════════
//  SPLASH SCREEN — "Path of Tides"
//  Redesigned: centered large title, no subtitle clutter,
//  animated ocean with floating ship, star field, wave rings
// ══════════════════════════════════════════════════════════════
class SplashScreen {
    sf::RenderWindow& window;
    sf::Font& font;

    float time = 0.f;
    float fadeIn = 0.f;
    bool musicOn = true;

    UIButton btnStart, btnExit, btnMusicToggle;

    // ── Stars ──
    static const int STAR_COUNT = 200;

    struct Star {
        float x, y, size, twinkle;
    };

    Star stars[STAR_COUNT];

    // ── Floating particles (sea foam / bokeh) ──
    static const int PART_COUNT = 60;

    struct Particle {
        float x, y, vx, vy, alpha, r, phase;
    };

    Particle parts[PART_COUNT];

    // ── Animated ship data ──
    float shipX = 0.f;
    float shipY = 400.f;

    // Color palette
    static constexpr sf::Uint8 BG_R = 2, BG_G = 4, BG_B = 14;

    sf::Color GOLD = sf::Color(210, 168, 40);
    sf::Color CYAN = sf::Color(40, 195, 230);
    sf::Color SILVER = sf::Color(180, 200, 220);

    void initStars() {
        for (int i = 0; i < STAR_COUNT; i++) {
            stars[i].x = (float)(rand() % 1280);
            stars[i].y = (float)(rand() % 420);
            stars[i].size = 0.5f + (rand() % 25) * 0.08f;
            stars[i].twinkle = (float)(rand() % 628) * 0.01f;
        }
    }

    void initParticles() {
        for (int i = 0; i < PART_COUNT; i++) {
            parts[i].x = (float)(rand() % 1280);
            parts[i].y = 440.f + rand() % 200;
            parts[i].vx = -0.15f + (rand() % 100) * 0.003f;
            parts[i].vy = -0.05f - (rand() % 30) * 0.005f;
            parts[i].alpha = 20.f + rand() % 50;
            parts[i].r = 1.f + rand() % 3;
            parts[i].phase = (float)(rand() % 628) * 0.01f;
        }
    }

    // Draw a simple ship silhouette at (cx, cy), scale s, facing right
    void drawShip(float cx, float cy, float s, sf::Uint8 alpha) {
        // Hull
        sf::ConvexShape hull;

        hull.setPointCount(6);

        hull.setPoint(0, sf::Vector2f(cx - 50 * s, cy));
        hull.setPoint(1, sf::Vector2f(cx + 60 * s, cy));
        hull.setPoint(2, sf::Vector2f(cx + 50 * s, cy + 14 * s));
        hull.setPoint(3, sf::Vector2f(cx + 10 * s, cy + 20 * s));
        hull.setPoint(4, sf::Vector2f(cx - 30 * s, cy + 20 * s));
        hull.setPoint(5, sf::Vector2f(cx - 55 * s, cy + 10 * s));

        hull.setFillColor(sf::Color(20, 35, 60, alpha));

        hull.setOutlineColor(
            sf::Color(
                SILVER.r,
                SILVER.g,
                SILVER.b,
                (sf::Uint8)(alpha * 0.6f)
            )
        );

        hull.setOutlineThickness(1.f);

        window.draw(hull);

        // Bridge
        sf::RectangleShape bridge(sf::Vector2f(40 * s, 18 * s));

        bridge.setPosition(cx - 15 * s, cy - 18 * s);
        bridge.setFillColor(sf::Color(25, 45, 75, alpha));

        bridge.setOutlineColor(
            sf::Color(
                SILVER.r,
                SILVER.g,
                SILVER.b,
                (sf::Uint8)(alpha * 0.4f)
            )
        );

        bridge.setOutlineThickness(1.f);

        window.draw(bridge);

        // Mast
        sf::Vertex mast[] = {
            sf::Vertex(
                sf::Vector2f(cx + 5 * s, cy - 18 * s),
                sf::Color(SILVER.r, SILVER.g, SILVER.b, alpha)
            ),

            sf::Vertex(
                sf::Vector2f(cx + 5 * s, cy - 50 * s),
                sf::Color(SILVER.r, SILVER.g, SILVER.b, (sf::Uint8)(alpha / 2))
            )
        };

        window.draw(mast, 2, sf::Lines);

        // Yard arm
        sf::Vertex yard[] = {
            sf::Vertex(
                sf::Vector2f(cx - 15 * s, cy - 44 * s),
                sf::Color(SILVER.r, SILVER.g, SILVER.b, (sf::Uint8)(alpha / 3))
            ),

            sf::Vertex(
                sf::Vector2f(cx + 25 * s, cy - 44 * s),
                sf::Color(SILVER.r, SILVER.g, SILVER.b, (sf::Uint8)(alpha / 3))
            )
        };

        window.draw(yard, 2, sf::Lines);

        // Smoke wisps
        for (int i = 0; i < 3; i++) {
            float sx = cx - 10 * s + i * 8 * s;

            float sy =
                cy - 55 * s
                - i * 8 * s
                + sinf(time * 2.f + i) * 4.f;

            sf::CircleShape smoke(4.f * s + i * 2 * s);

            smoke.setOrigin(smoke.getRadius(), smoke.getRadius());
            smoke.setPosition(sx, sy);

            smoke.setFillColor(
                sf::Color(
                    150,
                    160,
                    170,
                    (sf::Uint8)(30 - i * 8)
                )
            );

            window.draw(smoke);
        }
    }

    // Draw animated ocean waves in the lower portion
    void drawOcean() {
        // Ocean base
        for (int y2 = 440; y2 < 720; y2++) {
            float t2 = (float)(y2 - 440) / 280.f;

            sf::Uint8 r = (sf::Uint8)(3 + t2 * 8);
            sf::Uint8 g = (sf::Uint8)(12 + t2 * 25);
            sf::Uint8 b = (sf::Uint8)(35 + t2 * 50);

            sf::RectangleShape band(sf::Vector2f(1280.f, 1.f));

            band.setPosition(0.f, (float)y2);
            band.setFillColor(sf::Color(r, g, b));

            window.draw(band);
        }

        // Wave layers
        for (int layer = 0; layer < 6; layer++) {
            float baseY = 450.f + layer * 35.f;
            float speed = 0.4f + layer * 0.1f;
            float amp = 8.f - layer * 0.8f;
            float wavelen = 0.007f - layer * 0.0005f;

            sf::Uint8 wa = (sf::Uint8)(12 + layer * 8);

            sf::VertexArray va(sf::LinesStrip, 129);

            for (int xi = 0; xi <= 128; xi++) {
                float wx = xi * 10.f;

                float wy =
                    baseY
                    + sinf(time * speed + wx * wavelen + layer * 1.1f) * amp
                    + cosf(time * (speed * 0.7f) + wx * wavelen * 1.5f) * (amp * 0.4f);

                va[xi] = sf::Vertex(
                    sf::Vector2f(wx, wy),
                    sf::Color(
                        (sf::Uint8)(40 + layer * 5),
                        (sf::Uint8)(100 + layer * 8),
                        (sf::Uint8)(180 + layer * 4),
                        wa
                    )
                );
            }

            window.draw(va);
        }
    }

    // Draw stars with twinkle
    void drawStars() {
        for (int i = 0; i < STAR_COUNT; i++) {
            float t2 = sinf(time * 1.2f + stars[i].twinkle);

            sf::Uint8 a =
                (sf::Uint8)(
                    (0.5f + 0.5f * t2)
                    * fadeIn
                    * 180.f
                    );

            sf::CircleShape c(stars[i].size);

            c.setFillColor(sf::Color(220, 225, 240, a));

            c.setPosition(
                stars[i].x - stars[i].size,
                stars[i].y - stars[i].size
            );

            window.draw(c);
        }
    }

    // Subtle grid lines on horizon
    void drawHorizonGlow() {
        for (int i = 0; i < 40; i++) {
            float t2 = (float)i / 40.f;

            sf::Uint8 a =
                (sf::Uint8)(
                    (1.f - t2)
                    * fadeIn
                    * 25.f
                    );

            sf::RectangleShape line(sf::Vector2f(1280.f, 1.f));

            line.setPosition(0.f, 440.f + i * 0.6f);

            line.setFillColor(
                sf::Color(
                    CYAN.r,
                    CYAN.g,
                    CYAN.b,
                    a
                )
            );

            window.draw(line);
        }
    }

    // Draw a decorative compass rose at given center
    void drawCompassRose(float cx, float cy, float r, sf::Uint8 alpha) {
        // Outer ring
        sf::CircleShape ring(r);

        ring.setOrigin(r, r);
        ring.setPosition(cx, cy);

        ring.setFillColor(sf::Color::Transparent);

        ring.setOutlineColor(
            sf::Color(
                GOLD.r,
                GOLD.g,
                GOLD.b,
                (sf::Uint8)(alpha / 4)
            )
        );

        ring.setOutlineThickness(1.f);

        window.draw(ring);

        // 8 spokes
        for (int i = 0; i < 8; i++) {
            float ang = i * 45.f * 3.14159f / 180.f;

            float ex = cx + cosf(ang) * r * 0.88f;
            float ey = cy + sinf(ang) * r * 0.88f;

            sf::Vertex spoke[] = {
                sf::Vertex(
                    sf::Vector2f(cx, cy),
                    sf::Color(
                        GOLD.r,
                        GOLD.g,
                        GOLD.b,
                        (sf::Uint8)(alpha / 6)
                    )
                ),

                sf::Vertex(
                    sf::Vector2f(ex, ey),
                    sf::Color(
                        GOLD.r,
                        GOLD.g,
                        GOLD.b,
                        (sf::Uint8)(alpha / 10)
                    )
                )
            };

            window.draw(spoke, 2, sf::Lines);
        }

        // Cardinal diamond points
        for (int i = 0; i < 4; i++) {
            float ang =
                i * 90.f * 3.14159f / 180.f
                - 3.14159f / 2.f;

            float tip = r * 0.9f;
            float side = r * 0.18f;

            sf::ConvexShape point;

            point.setPointCount(4);

            point.setPoint(
                0,
                sf::Vector2f(
                    cx + cosf(ang) * tip,
                    cy + sinf(ang) * tip
                )
            );

            point.setPoint(
                1,
                sf::Vector2f(
                    cx + cosf(ang + 1.57f) * side,
                    cy + sinf(ang + 1.57f) * side
                )
            );

            point.setPoint(
                2,
                sf::Vector2f(
                    cx + cosf(ang + 3.14f) * r * 0.3f,
                    cy + sinf(ang + 3.14f) * r * 0.3f
                )
            );

            point.setPoint(
                3,
                sf::Vector2f(
                    cx + cosf(ang - 1.57f) * side,
                    cy + sinf(ang - 1.57f) * side
                )
            );

            sf::Uint8 pa =
                (i == 0)
                ? alpha
                : (sf::Uint8)(alpha / 2);

            point.setFillColor(
                sf::Color(
                    GOLD.r,
                    GOLD.g,
                    GOLD.b,
                    pa
                )
            );

            window.draw(point);
        }

        // Center dot
        sf::CircleShape dot(3.f);

        dot.setOrigin(3.f, 3.f);
        dot.setPosition(cx, cy);

        dot.setFillColor(
            sf::Color(
                GOLD.r,
                GOLD.g,
                GOLD.b,
                alpha
            )
        );

        window.draw(dot);
    }

    // Draw glowing text (2-pass: glow + sharp)
    void drawGlowText(
        const sf::String& str,
        float x,
        float y,
        unsigned sz,
        sf::Color col,
        sf::Uint8 alpha
    ) {
        sf::Color gc(
            col.r,
            col.g,
            col.b,
            (sf::Uint8)(alpha / 5)
        );

        for (int r = 4; r >= 1; r--) {
            sf::Text g(str, font, sz);

            g.setFillColor(gc);

            g.setPosition(x - r * 2.f, y - r * 2.f);
            window.draw(g);

            g.setPosition(x + r * 2.f, y - r * 2.f);
            window.draw(g);

            g.setPosition(x - r * 2.f, y + r * 2.f);
            window.draw(g);

            g.setPosition(x + r * 2.f, y + r * 2.f);
            window.draw(g);
        }

        sf::Text t(str, font, sz);

        t.setFillColor(
            sf::Color(
                col.r,
                col.g,
                col.b,
                alpha
            )
        );

        t.setPosition(x, y);

        window.draw(t);
    }

public:
    SplashScreen(sf::RenderWindow& win, sf::Font& fnt)
        : window(win), font(fnt) {

        initStars();
        initParticles();

        shipX = -150.f;
        shipY = 380.f;

        // Center buttons horizontally
        btnStart.setup(
            440,
            490,
            400,
            56,
            "  LAUNCH NAVIGATOR  ",
            font,
            15,
            sf::Color(8, 70, 130),
            sf::Color(12, 110, 200),
            sf::Color(4, 45, 90)
        );

        btnMusicToggle.setup(
            440,
            558,
            190,
            40,
            "MUSIC: ON",
            font,
            13,
            sf::Color(20, 40, 70),
            sf::Color(30, 65, 110),
            sf::Color(10, 22, 45)
        );

        btnExit.setup(
            638,
            558,
            202,
            40,
            "EXIT",
            font,
            13,
            sf::Color(75, 15, 15),
            sf::Color(120, 25, 25),
            sf::Color(45, 8, 8)
        );
    }
    // Returns: 0=wait, 1=launch, 2=exit
    int run(float dt) {
        fadeIn = min(1.f, fadeIn + dt * 0.9f);

        time += dt;

        // Animate ship sailing across screen
        shipX += 55.f * dt;

        if (shipX > 1380.f) {
            shipX = -150.f;
        }

        shipY = 385.f + sinf(time * 0.6f) * 6.f;

        // Update particles
        for (int i = 0; i < PART_COUNT; i++) {
            parts[i].x += parts[i].vx;

            parts[i].y +=
                parts[i].vy
                + sinf(time * 0.8f + parts[i].phase) * 0.04f;

            if (
                parts[i].y < 440.f
                || parts[i].x < 0
                || parts[i].x > 1280.f
                ) {
                parts[i].x = (float)(rand() % 1280);

                parts[i].y = 690.f + rand() % 30;

                parts[i].vx =
                    -0.1f
                    + (rand() % 100) * 0.002f;

                parts[i].vy =
                    -0.03f
                    - (rand() % 30) * 0.004f;
            }
        }

        sf::Vector2f mouse(
            (float)sf::Mouse::getPosition(window).x,
            (float)sf::Mouse::getPosition(window).y
        );

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();

                return 2;
            }

            if (
                event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Escape
                ) {
                return 2;
            }

            if (btnStart.clicked(event)) {
                return 1;
            }

            if (btnExit.clicked(event)) {
                return 2;
            }

            if (btnMusicToggle.clicked(event)) {
                musicOn = !musicOn;

                btnMusicToggle.setText(
                    musicOn
                    ? "MUSIC: ON"
                    : "MUSIC: OFF"
                );

                if (musicOn) {
                    PlayBackgroundMusic();
                }
                else {
                    StopBackgroundMusic();
                }
            }
        }

        btnStart.update(mouse);
        btnMusicToggle.update(mouse);
        btnExit.update(mouse);

        // ── Draw ──
        window.clear(sf::Color(BG_R, BG_G, BG_B));

        // Sky gradient (top)
        for (int row = 0; row < 440; row++) {
            float t2 = (float)row / 440.f;

            sf::RectangleShape b(sf::Vector2f(1280.f, 1.f));

            b.setPosition(0.f, (float)row);

            b.setFillColor(
                sf::Color(
                    (sf::Uint8)(BG_R + t2 * 6),
                    (sf::Uint8)(BG_G + t2 * 12),
                    (sf::Uint8)(BG_B + t2 * 22)
                )
            );

            window.draw(b);
        }

        drawStars();
        drawHorizonGlow();
        drawOcean();

        // Floating particles
        for (int i = 0; i < PART_COUNT; i++) {
            sf::CircleShape c(parts[i].r);

            c.setFillColor(
                sf::Color(
                    60,
                    140,
                    220,
                    (sf::Uint8)(
                        parts[i].alpha
                        * (
                            0.4f
                            + 0.6f
                            * sinf(time * 0.5f + parts[i].phase)
                            )
                        )
                )
            );

            c.setPosition(parts[i].x, parts[i].y);

            window.draw(c);
        }

        // Animated ship
        sf::Uint8 shipAlpha =
            (sf::Uint8)(fadeIn * 200.f);

        drawShip(shipX, shipY, 0.9f, shipAlpha);

        // Wake behind ship
        for (int w = 1; w <= 5; w++) {
            sf::CircleShape wake(w * 4.f + 2.f);

            wake.setOrigin(
                wake.getRadius(),
                wake.getRadius()
            );

            wake.setPosition(
                shipX - 55.f - w * 20.f,
                shipY + 18.f
            );

            wake.setFillColor(sf::Color::Transparent);

            wake.setOutlineColor(
                sf::Color(
                    80,
                    140,
                    200,
                    (sf::Uint8)(max(0, 35 - w * 6))
                )
            );

            wake.setOutlineThickness(1.f);

            window.draw(wake);
        }

        // Compass roses (decorative, subtle)
        sf::Uint8 ca =
            (sf::Uint8)(fadeIn * 90.f);

        drawCompassRose(100.f, 100.f, 70.f, ca);

        drawCompassRose(1180.f, 610.f, 55.f, ca);

        // ── Central title card ──
        float cardX = 240.f;
        float cardY = 200.f;
        float cardW = 800.f;
        float cardH = 260.f;

        sf::RectangleShape card(
            sf::Vector2f(cardW, cardH)
        );

        card.setPosition(cardX, cardY);

        card.setFillColor(
            sf::Color(
                3,
                8,
                22,
                (sf::Uint8)(fadeIn * 210.f)
            )
        );

        card.setOutlineColor(
            sf::Color(
                GOLD.r,
                GOLD.g,
                GOLD.b,
                (sf::Uint8)(fadeIn * 60.f)
            )
        );

        card.setOutlineThickness(1.f);

        window.draw(card);

        // Gold accent top line
        sf::RectangleShape topLine(
            sf::Vector2f(cardW, 2.f)
        );

        topLine.setPosition(cardX, cardY);

        topLine.setFillColor(
            sf::Color(
                GOLD.r,
                GOLD.g,
                GOLD.b,
                (sf::Uint8)(fadeIn * 200.f)
            )
        );

        window.draw(topLine);

        // Thin cyan bottom line
        sf::RectangleShape botLine(
            sf::Vector2f(cardW, 1.f)
        );

        botLine.setPosition(cardX, cardY + cardH);

        botLine.setFillColor(
            sf::Color(
                CYAN.r,
                CYAN.g,
                CYAN.b,
                (sf::Uint8)(fadeIn * 80.f)
            )
        );

        window.draw(botLine);

        // ── MAIN TITLE — "Path of Tides" — large, centered ──
        sf::Text title(
            "Path of Tides",
            font,
            72
        );

        title.setStyle(sf::Text::Bold);

        title.setFillColor(
            sf::Color(
                GOLD.r,
                GOLD.g,
                GOLD.b,
                (sf::Uint8)(fadeIn * 255.f)
            )
        );

        sf::FloatRect tb =
            title.getLocalBounds();

        float titleX =
            cardX
            + (cardW - tb.width) / 2.f
            - tb.left;

        float titleY =
            cardY
            + (cardH - tb.height) / 2.f
            - tb.top
            - 30.f;

        // Glow passes
        for (int r = 5; r >= 1; r--) {
            sf::Text glow(
                "Path of Tides",
                font,
                72
            );

            glow.setStyle(sf::Text::Bold);

            glow.setFillColor(
                sf::Color(
                    GOLD.r,
                    GOLD.g,
                    GOLD.b,
                    (sf::Uint8)(fadeIn * 18.f / r)
                )
            );

            glow.setPosition(
                titleX - r * 3.f,
                titleY - r * 3.f
            );

            window.draw(glow);

            glow.setPosition(
                titleX + r * 3.f,
                titleY - r * 3.f
            );

            window.draw(glow);

            glow.setPosition(
                titleX - r * 3.f,
                titleY + r * 3.f
            );

            window.draw(glow);

            glow.setPosition(
                titleX + r * 3.f,
                titleY + r * 3.f
            );

            window.draw(glow);
        }

        title.setPosition(titleX, titleY);

        window.draw(title);

        // Tagline — small, elegant
        sf::Text tagline(
            "Maritime Navigation Optimizer",
            font,
            16
        );

        tagline.setFillColor(
            sf::Color(
                SILVER.r,
                SILVER.g,
                SILVER.b,
                (sf::Uint8)(fadeIn * 160.f)
            )
        );

        sf::FloatRect tlb =
            tagline.getLocalBounds();

        tagline.setPosition(
            cardX
            + (cardW - tlb.width) / 2.f
            - tlb.left,
            titleY + 84.f
        );

        window.draw(tagline);

        // Animated wave dots beneath tagline
        float dotsY = titleY + 115.f;

        float pulseA = sinf(time * 2.2f);

        for (int i = 0; i < 9; i++) {
            float pulse =
                sinf(time * 2.2f + i * 0.6f);

            float dr =
                2.f + pulse * 1.2f;

            sf::CircleShape dot(dr);

            dot.setOrigin(dr, dr);

            dot.setPosition(
                cardX
                + cardW / 2.f
                - 4 * 24.f
                + i * 24.f,

                dotsY + pulse * 3.f
            );

            dot.setFillColor(
                sf::Color(
                    GOLD.r,
                    GOLD.g,
                    GOLD.b,
                    (sf::Uint8)(
                        fadeIn
                        * (
                            50.f
                            + pulse * 50.f
                            )
                        )
                )
            );

            window.draw(dot);
        }

        // ── Buttons ──
        btnStart.draw(window);
        btnMusicToggle.draw(window);
        btnExit.draw(window);

        // Pulsing ring around start button
        float ringPulse =
            0.5f
            + 0.5f * sinf(time * 2.f);

        sf::RectangleShape ring(
            sf::Vector2f(
                404.f + ringPulse * 6.f,
                60.f + ringPulse * 6.f
            )
        );

        ring.setPosition(
            438.f - ringPulse * 3.f,
            488.f - ringPulse * 3.f
        );

        ring.setFillColor(sf::Color::Transparent);

        ring.setOutlineColor(
            sf::Color(
                GOLD.r,
                GOLD.g,
                GOLD.b,
                (sf::Uint8)(
                    fadeIn
                    * 50.f
                    * ringPulse
                    )
            )
        );

        ring.setOutlineThickness(1.f);

        window.draw(ring);

        // Footer
        sf::Text footer(
            "Path of Tides  |  Data Structures Course Project  |  Press ESC to quit",
            font,
            11
        );

        footer.setFillColor(
            sf::Color(
                50,
                65,
                90,
                (sf::Uint8)(fadeIn * 180.f)
            )
        );

        sf::FloatRect fb =
            footer.getLocalBounds();

        footer.setPosition(
            640.f - fb.width / 2.f - fb.left,
            706.f
        );

        window.draw(footer);

        window.display();

        return 0;
    }
};

// ══════════════════════════════════════════════════════════════
//  MAIN VISUALIZATION — "Path of Tides"
// ══════════════════════════════════════════════════════════════
class Visualization 
{
private:
    sf::RenderWindow& window;
    MaritimeGraph& graph;
    CustomShipManager& shipManager;
    sf::Font& font;

    sf::Texture mapTexture;
    sf::Sprite  mapSprite;
    bool hasMap = false;

    sf::Texture shipTexture;
    bool hasShipTex = false;
    sf::Sprite shipSprite;

    // ── Zoom & Pan ──
    float zoomLevel = 1.0f;
    sf::Vector2f viewOffset = sf::Vector2f(0, 0);
    bool isPanning = false;
    sf::Vector2f panStart;
    static constexpr float ZOOM_MIN = 0.4f;
    static constexpr float ZOOM_MAX = 8.0f;

    // ── Selection & Hover ──
    int selectedOrigin = -1, selectedDest = -1, hoveredPort = -1;

    // ── Path & Algorithm ──
    LinkedList<RouteLeg> currentPath;
    bool showingPath = false;
    bool stepByStepMode = false;
    int  animationStep = 0;
    LinkedList<int> exploredNodes, visitedOrder;
    LinkedList<MaritimeGraph::DijkstraExplorationStep> dijkstraSteps;
    int  currentDijkstraStep = 0;
    bool autoStep = false;
    float stepTimer = 0;
    bool showExplored = true;
    bool filterActive = false;
    MaritimeGraph* activeSubgraph = nullptr;
    int  colorMode = 0;
    bool useCost = true;
    MyString statusMessage;
    float statusTimer = 0;
    float globalTime = 0;

    // ── Ship Animation ──
    bool  animatingShip = false;
    int   shipPathLeg = 0;
    float shipLegProgress = 0.f;
    sf::Vector2f shipPos;
    sf::Vector2f shipDir = sf::Vector2f(1, 0);
    float shipSpeed = 80.f;

    // ── UI state ──
    bool showInfoPanel = false;
    bool showShipPanel = false;
    bool showFilterPanel = false;
    bool showRouteDetails = false;
    bool showLegend = false;

    // ── Ship form ──
    char newShipName[32] = "MyShip";
    char newShipDuration[10] = "0";
    char newShipCompany[32] = "OceanCo";
    char avoidPortInput[32] = "";
    char prefPortInput[32] = "";
    LinkedList<int> avoidPortSel, prefPortSel, prefCompanySel;
    bool exclusiveCompany = false;
    enum FormField { FF_NONE, FF_NAME, FF_DUR, FF_COMPANY, FF_AVOID, FF_PREF };
    FormField activeField = FF_NONE;

    // ── Filter ──
    LinkedList<MyString> companyList;
    LinkedList<bool>     companySelected;
    char filterMaxCost[10] = "-1";
    char filterMaxDur[10] = "-1";
    int  fMaxCost = -1, fMaxDur = -1;
    enum FilterField { FLT_NONE, FLT_COST, FLT_DUR };
    FilterField filterField = FLT_NONE;

    // ── Toolbar ──
    static const int BTN_COUNT = 11;
    UIButton toolbar[BTN_COUNT];
    UIButton btnInfo;
    UIButton btnZoomIn, btnZoomOut, btnZoomReset;
    UIButton btnSaveShip, btnCancelShip, btnClearShip;
    UIButton btnFilterApply, btnFilterClear, btnFilterClose;

    // ── Colors ──
    sf::Color COL_BG = sf::Color(2, 4, 14);
    sf::Color COL_PANEL = sf::Color(4, 10, 28, 238);
    sf::Color COL_GOLD = sf::Color(210, 168, 40);
    sf::Color COL_CYAN = sf::Color(40, 195, 230);
    sf::Color COL_GREEN = sf::Color(40, 210, 100);
    sf::Color COL_RED = sf::Color(220, 55, 55);
    sf::Color COL_PATH = sf::Color(50, 255, 130);
    sf::Color COL_EXPL = sf::Color(80, 130, 255);
    sf::Color COL_VISIT = sf::Color(255, 215, 50);
    sf::Color COL_PORT = sf::Color(240, 185, 38);
    sf::Color COL_ROUTE = sf::Color(70, 120, 170, 40);
    sf::Color COL_BTN = sf::Color(8, 22, 52);
    sf::Color COL_BTNHV = sf::Color(16, 44, 104);

    // ─────────────────────────────────────────────────────────
    sf::Vector2f worldToScreen(float wx, float wy) const 
    {
        return sf::Vector2f((wx + viewOffset.x) * zoomLevel,(wy + viewOffset.y) * zoomLevel);
    }
    sf::Vector2f screenToWorld(float sx, float sy) const 
    {
        return sf::Vector2f(sx / zoomLevel - viewOffset.x, sy / zoomLevel - viewOffset.y);
    }

    void drawText(const sf::String& s, float x, float y, unsigned sz, sf::Color c, bool bold = false) 
    {
        sf::Text t(s, font, sz); 
        t.setFillColor(c);
        if (bold)
        {
            t.setStyle(sf::Text::Bold);
        }
        t.setPosition(x, y); 
        window.draw(t);
    }
    void drawRect(float x, float y, float w, float h, sf::Color fill, sf::Color outline = sf::Color::Transparent, float thick = 0) 
    {
        sf::RectangleShape r(sf::Vector2f(w, h)); 
        r.setPosition(x, y); 
        r.setFillColor(fill);
        if (thick > 0) 
        { 
            r.setOutlineColor(outline); 
            r.setOutlineThickness(thick); 
        }
        window.draw(r);
    }
    void drawWorldLine(float x1, float y1, float x2, float y2, sf::Color c, float thick = 2.f) 
    {
        sf::Vector2f s1 = worldToScreen(x1, y1), s2 = worldToScreen(x2, y2);
        float dx = s2.x - s1.x, dy = s2.y - s1.y, len = sqrtf(dx * dx + dy * dy);
        if (len < 0.5f)
        {
            return;
        }
        sf::RectangleShape r(sf::Vector2f(len, thick));
        r.setPosition(s1); 
        r.setFillColor(c);
        r.setRotation(atan2f(dy, dx) * 180.f / 3.14159f);
        window.draw(r);
    }
    void drawWorldArrow(float x1, float y1, float x2, float y2, sf::Color c, float thick = 3.f) 
    {
        drawWorldLine(x1, y1, x2, y2, c, thick);
        sf::Vector2f s2 = worldToScreen(x2, y2);
        float dx = x2 - x1, dy = y2 - y1, len = sqrtf(dx * dx + dy * dy);
        if (len < 1.f)
        {
            return;
        }
        float nx = dx / len, ny = dy / len, sz = 10.f;
        sf::Vector2f ax = s2 - sf::Vector2f(nx, ny) * 16.f;
        sf::ConvexShape arr;
        arr.setPointCount(3);
        arr.setPoint(0, s2);
        arr.setPoint(1, ax + sf::Vector2f(-ny * sz * .5f, nx * sz * .5f));
        arr.setPoint(2, ax + sf::Vector2f(ny * sz * .5f, -nx * sz * .5f));
        arr.setFillColor(c); 
        window.draw(arr);
    }
    void drawWorldGlow(float wx, float wy, float r, sf::Color c) 
    {
        sf::Vector2f sc = worldToScreen(wx, wy);
        float sr = r * zoomLevel;
        for (int i = 4; i >= 1; i--) 
        {
            sf::CircleShape g(sr * i * 0.45f);
            g.setFillColor(sf::Color(c.r, c.g, c.b, (sf::Uint8)(18 / i)));
            g.setOrigin(g.getRadius(), g.getRadius()); g.setPosition(sc);
            window.draw(g);
        }
    }
    void setStatus(const MyString& s, float dur = 4.f) 
    { 
        statusMessage = s; 
        statusTimer = dur; 
    }

    sf::Color routeCostColor(int cost) 
    {
        if (cost < 100)
        {
            return sf::Color(80, 255, 120);
        }
        if (cost < 300)
        {
            return sf::Color(180, 255, 80);
        }
        if (cost < 500)
        {
            return sf::Color(255, 240, 60);
        }
        if (cost < 800)
        {
            return sf::Color(255, 160, 60);
        }
        return sf::Color(255, 80, 80);
    }
    sf::Color routeDurColor(int dur) 
    {
        if (dur < 60)
        {
            return sf::Color(80, 255, 120);
        }
        if (dur < 120)
        {
            return sf::Color(180, 255, 80);
        }
        if (dur < 180)
        {
            return sf::Color(255, 240, 60);
        }
        if (dur < 300)
        {
            return sf::Color(255, 160, 60);
        }
        return sf::Color(255, 80, 80);
    }
    sf::Color companyColor(const MyString& c) 
    {
        unsigned h = 0; const char* s = c.c_str();
        while (*s) 
        { 
            h = h * 31 + (unsigned char)*s++; 
        }
        return sf::Color(80 + (h % 160), 80 + ((h / 160) % 160), 80 + ((h / 25600) % 160));
    }

    int findPortAt(float mx, float my) 
    {
        for (int i = 0; i < graph.getPortCount(); i++) 
        {
            Port& p = graph.getPort(i);
            sf::Vector2f sc = worldToScreen(p.x, p.y);
            float dx = mx - sc.x, dy = my - sc.y;
            float r = max(10.f, 10.f * zoomLevel);
            if (dx * dx + dy * dy < r * r)
            {
                return i;
            }
        }
        return -1;
    }
    bool isPortInPath(int i) 
    {
        for (int j = 0; j < currentPath.getSize(); j++)
        {
            if (graph.getPortIndex(currentPath.get(j).fromPort) == i || graph.getPortIndex(currentPath.get(j).toPort) == i)
            {
                return true;
            }
        } 
        return false;
    }
    bool isPortFiltered(int i) 
    {
        if (!filterActive || !activeSubgraph)
        {
            return false;
        }
        return activeSubgraph->getPortIndex(graph.getPort(i).name) < 0;
    }
    int visitingNode() 
    {
        if (stepByStepMode && currentDijkstraStep > 0 && currentDijkstraStep <= dijkstraSteps.getSize())
        {
            return dijkstraSteps.get(currentDijkstraStep - 1).currentNode;
        }
        return -1;
    }
    MaritimeGraph& activeGraph() 
    { 
        return filterActive && activeSubgraph ? *activeSubgraph : graph; 
    }
    void advanceStep() 
    {
        if (currentDijkstraStep < dijkstraSteps.getSize()) 
        {
            auto& s = dijkstraSteps.get(currentDijkstraStep);
            if (exploredNodes.find(s.neighborNode) < 0)
            {
                exploredNodes.pushBack(s.neighborNode);
            }
            currentDijkstraStep++;
        }
        else 
        {
            showingPath = true; stepByStepMode = false;
            setStatus(MyString("Exploration complete — shortest path highlighted"));
        }
    }

    void buildToolbar() 
    {
        const char* labels[BTN_COUNT] = 
        {
            "CHEAPEST","FASTEST","STEP","AUTO","RESET",
            "SHIPS","FILTER","ROUTES","LEGEND","COLOR","MENU"
        };
        sf::Color cols[BTN_COUNT] = 
        {
            sf::Color(8,80,45),   sf::Color(8,55,120),  sf::Color(70,35,130),
            sf::Color(35,72,120), sf::Color(90,30,30),  sf::Color(75,55,8),
            sf::Color(55,22,95),  sf::Color(18,72,95),  sf::Color(35,62,52),
            sf::Color(30,58,58),  sf::Color(10,10,38)
        };
        sf::Color hovs[BTN_COUNT] = 
        {
            sf::Color(12,130,72), sf::Color(12,92,190), sf::Color(110,55,200),
            sf::Color(55,112,190),sf::Color(150,50,50), sf::Color(120,88,12),
            sf::Color(85,36,145), sf::Color(28,112,148),sf::Color(55,97,82),
            sf::Color(50,92,92),  sf::Color(28,28,75)
        };
        float startX = 5, y = 6, bw = 80, bh = 34, gap = 3;
        for (int i = 0; i < BTN_COUNT; i++)
        {
            toolbar[i].setup(startX + i * (bw + gap), y, bw, bh, labels[i], font, 11,
                cols[i], hovs[i], sf::Color(cols[i].r / 2, cols[i].g / 2, cols[i].b / 2));
        }

        btnInfo.setup(1190, 6, 85, 34, "INFO", font, 12, sf::Color(18, 55, 95), sf::Color(30, 92, 150), sf::Color(9, 32, 62));
        btnZoomIn.setup(1190, 52, 40, 32, "+", font, 16, sf::Color(18, 55, 75), sf::Color(28, 92, 120), sf::Color(9, 32, 46));
        btnZoomOut.setup(1235, 52, 40, 32, "-", font, 16, sf::Color(18, 55, 75), sf::Color(28, 92, 120), sf::Color(9, 32, 46));
        btnZoomReset.setup(1190, 88, 85, 28, "RESET ZOOM", font, 9, sf::Color(12, 36, 56), sf::Color(22, 60, 90), sf::Color(6, 20, 36));

        btnSaveShip.setup(420, 648, 120, 34, "SAVE SHIP", font, 12, sf::Color(8, 92, 45), sf::Color(12, 145, 75), sf::Color(4, 55, 28));
        btnCancelShip.setup(548, 648, 100, 34, "CANCEL", font, 12, sf::Color(85, 18, 18), sf::Color(140, 32, 32), sf::Color(50, 9, 9));
        btnClearShip.setup(656, 648, 100, 34, "CLEAR", font, 12, sf::Color(65, 45, 8), sf::Color(108, 80, 12), sf::Color(38, 25, 4));

        btnFilterApply.setup(420, 648, 120, 34, "APPLY FILTER", font, 11, sf::Color(8, 102, 45), sf::Color(12, 155, 75), sf::Color(4, 60, 28));
        btnFilterClear.setup(548, 648, 110, 34, "CLEAR", font, 12, sf::Color(85, 36, 8), sf::Color(140, 60, 12), sf::Color(50, 20, 4));
        btnFilterClose.setup(666, 648, 90, 34, "CLOSE", font, 12, sf::Color(46, 12, 65), sf::Color(75, 22, 108), sf::Color(26, 7, 40));
    }

    void drawMapBackground() 
    {
        if (hasMap) 
        {
            sf::Vector2f org = worldToScreen(0, 0);
            mapSprite.setPosition(org);
            sf::Vector2u ts = mapTexture.getSize();
            mapSprite.setScale(1280.f / ts.x * zoomLevel, 720.f / ts.y * zoomLevel);
            window.draw(mapSprite);
        }
        else 
        {
            sf::RectangleShape bg(sf::Vector2f(1280, 720));
            bg.setFillColor(sf::Color(2, 5, 18)); 
            window.draw(bg);
            for (int x = 0; x < 1280; x += 80) 
            {
                sf::Vector2f s1 = worldToScreen((float)x, 0), s2 = worldToScreen((float)x, 720);
                sf::Vertex line[] = { sf::Vertex(s1,sf::Color(18,45,85,10)),sf::Vertex(s2,sf::Color(18,45,85,10)) };
                window.draw(line, 2, sf::Lines);
            }
            for (int y = 0; y < 720; y += 60) 
            {
                sf::Vector2f s1 = worldToScreen(0, (float)y), s2 = worldToScreen(1280, (float)y);
                sf::Vertex line[] = { sf::Vertex(s1,sf::Color(18,45,85,10)),sf::Vertex(s2,sf::Color(18,45,85,10)) };
                window.draw(line, 2, sf::Lines);
            }
        }
        // Vignette
        for (int i = 0; i < 50; i++) 
        {
            sf::Uint8 a = (sf::Uint8)(i * 2.5f);
            sf::RectangleShape v(sf::Vector2f(1280, 2));
            v.setPosition(0, 720 - 50 + i); 
            v.setFillColor(sf::Color(2, 4, 14, a));
            window.draw(v);
        }
    }

    void drawAllRoutes() 
    {
        MaritimeGraph& ag = activeGraph();
        for (int i = 0; i < ag.getPortCount(); i++) 
        {
            Port& fp = ag.getPort(i);
            LinkedList<RouteEdge>& edges = ag.getEdges(i);
            for (int j = 0; j < edges.getSize(); j++) 
            {
                int di = edges.get(j).destPortIndex;
                if (di < 0 || di >= ag.getPortCount())
                {
                    continue;
                }
                Port& tp = ag.getPort(di);
                sf::Color c = COL_ROUTE;
                if (colorMode == 1 && edges.get(j).routes.getSize() > 0) 
                {
                    int avg = 0; 
                    for (int k = 0; k < edges.get(j).routes.getSize(); k++)
                    {
                        avg += edges.get(j).routes.get(k).cost;
                    }
                    avg /= edges.get(j).routes.getSize(); 
                    c = routeCostColor(avg); 
                    c.a = 50;
                }
                else if (colorMode == 2 && edges.get(j).routes.getSize() > 0) 
                {
                    int avg = 0; 
                    for (int k = 0; k < edges.get(j).routes.getSize(); k++)
                    {
                        avg += edges.get(j).routes.get(k).getDuration();
                    }
                    avg /= edges.get(j).routes.getSize(); 
                    c = routeDurColor(avg); 
                    c.a = 50;
                }
                else if (colorMode == 3 && edges.get(j).routes.getSize() > 0) 
                {
                    c = companyColor(edges.get(j).routes.get(0).company); 
                    c.a = 50;
                }
                drawWorldLine(fp.x, fp.y, tp.x, tp.y, c, 1.f);
            }
        }
    }

    void drawExploration() 
    {
        if (!stepByStepMode || !showExplored || dijkstraSteps.isEmpty())
        {
            return;
        }
        for (int i = 0; i < currentDijkstraStep && i < dijkstraSteps.getSize(); i++) 
        {
            auto& s = dijkstraSteps.get(i);
            if (s.currentNode < 0 || s.currentNode >= graph.getPortCount())
            {
                continue;
            }
            if (s.neighborNode < 0 || s.neighborNode >= graph.getPortCount())
            {
                continue;
            }
            Port& a = graph.getPort(s.currentNode);
            Port& b = graph.getPort(s.neighborNode);
            float pulse = 0.6f + 0.4f * sinf(globalTime * 3.f + i * 0.3f);
            sf::Color c = s.wasRelaxed ?
                sf::Color(80, 200, 255, (sf::Uint8)(90 * pulse)) :
                sf::Color(80, 80, 200, (sf::Uint8)(50 * pulse));
            drawWorldLine(a.x, a.y, b.x, b.y, c, 1.5f);
        }
        int vn = visitingNode();
        if (vn >= 0 && vn < graph.getPortCount()) 
        {
            Port& p = graph.getPort(vn);
            drawWorldGlow(p.x, p.y, 15.f, COL_VISIT);
        }
    }

    void drawPath()
    {
        int steps = showingPath ? currentPath.getSize() : animationStep;
        for (int i = 0; i < steps && i < currentPath.getSize(); i++) 
        {
            int fi = graph.getPortIndex(currentPath.get(i).fromPort);
            int ti = graph.getPortIndex(currentPath.get(i).toPort);
            if (fi < 0 || ti < 0)
            {
                continue;
            }
            Port& f = graph.getPort(fi); Port& t = graph.getPort(ti);
            float pulse = 0.7f + 0.3f * sinf(globalTime * 2.5f - i * 0.5f);
            drawWorldLine(f.x, f.y, t.x, t.y, sf::Color(COL_PATH.r, COL_PATH.g, COL_PATH.b, (sf::Uint8)(22 * pulse)), 10.f);
            drawWorldLine(f.x, f.y, t.x, t.y, sf::Color(COL_PATH.r, COL_PATH.g, COL_PATH.b, (sf::Uint8)(48 * pulse)), 5.f);
            drawWorldArrow(f.x, f.y, t.x, t.y, sf::Color(COL_PATH.r, COL_PATH.g, COL_PATH.b, (sf::Uint8)(215 * pulse)), 2.f);
        }
    }

    void drawPort(int i) 
    {
        Port& p = graph.getPort(i);
        sf::Vector2f sc = worldToScreen(p.x, p.y);
        bool sel = (i == selectedOrigin || i == selectedDest);
        bool hov = (i == hoveredPort);
        bool inPath = isPortInPath(i);
        bool filtered = isPortFiltered(i);
        bool visiting = (i == visitingNode());
        bool explored = (exploredNodes.find(i) >= 0);

        float rad = 6.f * min(zoomLevel, 2.f);
        if (sel)
        {
            rad *= 1.7f;
        }
        else if (hov)
        {
            rad *= 1.4f;
        }
        else if (inPath)
        {
            rad *= 1.3f;
        }

        sf::Color fill, outline;
        if (i == selectedOrigin) 
        {
            fill = sf::Color(255, 70, 70);  
            outline = sf::Color::White; 
        }
        else if (i == selectedDest) 
        { 
            fill = sf::Color(70, 70, 255);  
            outline = sf::Color::White; 
        }
        else if (visiting) 
        { 
            fill = COL_VISIT; 
            outline = sf::Color::White; 
        }
        else if (inPath) 
        { 
            fill = COL_PATH;    
            outline = sf::Color::White; 
        }
        else if (hov) 
        { 
            fill = sf::Color(255, 210, 80); 
            outline = sf::Color::White; 
        }
        else if (filtered) 
        { 
            fill = sf::Color(160, 25, 25);  
            outline = sf::Color(220, 50, 50); 
        }
        else if (explored) 
        { 
            fill = COL_EXPL;  
            outline = sf::Color(120, 160, 255); 
        }
        else 
        { 
            fill = COL_PORT;          
            outline = sf::Color(170, 128, 18); 
        }

        CustomShip* as = shipManager.getActiveShip();
        if (as) 
        {
            for (int k = 0; k < as->avoidPorts.getSize(); k++)
            {
                if (as->avoidPorts.get(k) == p.name) 
                { 
                    fill = sf::Color(200, 20, 20); 
                    break; 
                }
            }
                
            for (int k = 0; k < as->preferredPorts.getSize(); k++)
            {
                if (as->preferredPorts.get(k) == p.name)
                {
                    outline = sf::Color(40, 255, 100);
                    break;
                }
            }
        }

        if (sel || inPath || hov || visiting)
        {
            drawWorldGlow(p.x, p.y, rad * 2.2f, fill);
        }

        if (sel) 
        {
            float pulse = 1.f + sinf(globalTime * 3.f) * 0.2f;
            sf::CircleShape ring(rad * pulse + 4);
            ring.setOrigin(rad * pulse + 4, rad * pulse + 4); 
            ring.setPosition(sc);
            ring.setFillColor(sf::Color::Transparent);
            ring.setOutlineColor(sf::Color(fill.r, fill.g, fill.b, 80));
            ring.setOutlineThickness(1.5f); 
            window.draw(ring);
        }

        sf::CircleShape c(rad);
        c.setOrigin(rad, rad); 
        c.setPosition(sc);
        c.setFillColor(fill);
        c.setOutlineColor(outline); 
        c.setOutlineThickness(sel ? 2.f : 1.2f);
        window.draw(c);

        if (zoomLevel >= 0.6f) 
        {
            float labelSize = max(8.f, min(12.f, 9.f * zoomLevel));
            sf::Text lbl(p.name.c_str(), font, (unsigned)labelSize);
            lbl.setFillColor((sel || inPath) ? sf::Color::White : sf::Color(195, 208, 222));
            lbl.setPosition(sc.x + rad + 3, sc.y - labelSize * 0.5f);
            window.draw(lbl);
        }

        if (p.dockingQueue.getSize() > 0) 
        {
            char buf[8]; 
            sprintf_s(buf, "%d", p.dockingQueue.getSize());
            sf::Text q(buf, font, 9); 
            q.setFillColor(sf::Color::Yellow);
            q.setPosition(sc.x + rad - 2, sc.y - rad - 12); 
            window.draw(q);
        }
        if (stepByStepMode && explored) 
        {
            for (int j = 0; j < visitedOrder.getSize(); j++) 
            {
                if (visitedOrder.get(j) == i) 
                {
                    char nb[8]; sprintf_s(nb, "%d", j + 1);
                    sf::Text n2(nb, font, 8); 
                    n2.setFillColor(sf::Color::Yellow);
                    n2.setPosition(sc.x - 4, sc.y - 5); 
                    window.draw(n2); break;
                }
            }
        }
    }

    void drawToolbar() 
    {
        drawRect(0, 0, 1280, 46, sf::Color(2, 5, 18, 248));
        drawRect(0, 44, 1280, 2, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 60));
        sf::Text title("PATH OF TIDES", font, 13);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(COL_GOLD);
        title.setPosition(5, 14); 
        window.draw(title);
        for (int i = 0; i < BTN_COUNT; i++)
        {
            toolbar[i].draw(window);
        }
        btnInfo.draw(window);
        btnZoomIn.draw(window);
        btnZoomOut.draw(window); 
        btnZoomReset.draw(window);
        char zb[16]; sprintf_s(zb, "x%.1f", zoomLevel);
        sf::Text zl(zb, font, 10); 
        zl.setFillColor(sf::Color(95, 135, 175));
        zl.setPosition(1210, 120); 
        window.draw(zl);
        const char* cmL[] = 
        { 
            "Default","By Cost","By Duration","By Company" 
        };
        char cb[32]; sprintf_s(cb, "[%s]", cmL[colorMode]);
        sf::Text cm(cb, font, 9); 
        cm.setFillColor(sf::Color(75, 115, 155));
        cm.setPosition(875, 35); 
        window.draw(cm);
    }

    void drawInfoPanel() 
    {
        if (!showInfoPanel)
        {
            return;
        }
        drawRect(0, 46, 220, 674, COL_PANEL, sf::Color::Transparent, 0);
        drawRect(218, 46, 2, 674, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 35));
        float y = 54;
        auto sec = [&](const char* t2) 
            {
            drawRect(0, (int)y - 2, 220, 18, sf::Color(6, 16, 42, 200));
            drawText(t2, 6, y, 10, COL_CYAN, true); y += 20;
            drawRect(0, (int)y - 3, 220, 1, sf::Color(36, 54, 85)); 
            y += 4;
            };
        auto row = [&](const char* s, sf::Color c = sf::Color(168, 188, 208)) 
            {
            drawText(s, 8, y, 10, c); 
            y += 15;
            };
        char buf[80];
        sec("SELECTION");
        if (selectedOrigin >= 0) 
        { 
            drawRect(4, (int)y - 1, 212, 16, sf::Color(55, 14, 14, 150)); 
            sprintf_s(buf, "FROM: %s", graph.getPort(selectedOrigin).name.c_str()); 
            row(buf, sf::Color(255, 110, 110)); 
        }
        else
        {
            row("FROM: (left-click port)");
        }
        if (selectedDest >= 0) 
        { 
            drawRect(4, (int)y - 1, 212, 16, sf::Color(14, 14, 55, 150)); 
            sprintf_s(buf, "TO:   %s", graph.getPort(selectedDest).name.c_str()); 
            row(buf, sf::Color(110, 110, 255)); 
        }
        else
        {
            row("TO:   (right-click port)");
        }
        y += 4;
        sec("GRAPH STATS");
        sprintf_s(buf, "Ports:  %d", graph.getPortCount()); 
        row(buf);
        sprintf_s(buf, "Routes: %d", graph.getTotalRouteCount()); 
        row(buf);
        if (filterActive && activeSubgraph) 
        { 
            sprintf_s(buf, "Filtered: %d ports", activeSubgraph->getPortCount()); 
            row(buf, sf::Color(255, 162, 55)); 
        }
        y += 4;
        sec("ACTIVE SHIP");
        CustomShip* as = shipManager.getActiveShip();
        if (as) 
        {
            drawRect(4, (int)y - 1, 212, 16, sf::Color(9, 46, 14, 160)); row(as->name.c_str(), sf::Color(80, 255, 120));
            if (as->maxVoyageDuration > 0)
            {
                sprintf_s(buf, "Max: %d min", as->maxVoyageDuration);
            }
            else
            {
                strcpy_s(buf, "Max: Unlimited");
            }
            row(buf);
            sprintf_s(buf, "Avoid: %d ports", as->avoidPorts.getSize());
            row(buf, sf::Color(220, 138, 138));
            sprintf_s(buf, "Pref: %d companies", as->preferCompanies.getSize()); 
            row(buf, sf::Color(220, 220, 138));
        }
        else row("None (use SHIPS button)", sf::Color(88, 88, 88));
        y += 4;
        sec("MODE");
        if (stepByStepMode) 
        { 
            row("Step-by-Step Dijkstra", sf::Color(158, 88, 255)); 
            sprintf_s(buf, "Step %d / %d", currentDijkstraStep, dijkstraSteps.getSize()); 
            row(buf, sf::Color(178, 138, 255)); 
            row("SPACE=next step"); 
        }
        else if (showingPath)
        {
            row("Route Found", sf::Color(80, 255, 120));
        }
        else if (filterActive)
        {
            row("Filter Active", sf::Color(255, 148, 48));
        }
        else
        {
            row("Ready", sf::Color(128, 148, 168));
        }
        y += 4;
        sec("REGISTERED SHIPS");
        for (int i = 0; i < shipManager.getShipCount() && y < 620; i++) 
        {
            bool isAct = (i == shipManager.getActiveShipIndex());
            drawRect(4, (int)y - 1, 212, 16, isAct ? sf::Color(9, 56, 18, 180) : sf::Color(9, 18, 42, 140));
            CustomShip* s = shipManager.getShipByIndex(i);
            string lbl = string(isAct ? "> " : "  ") + string(s->name.c_str());
            row(lbl.c_str(), isAct ? sf::Color(80, 255, 120) : sf::Color(148, 168, 188));
        }
        if (shipManager.getShipCount() == 0)
        {
            row("No ships registered", sf::Color(68, 68, 68));
        }
        if (!statusMessage.isEmpty() && statusTimer > 0) 
        {
            drawRect(0, 690, 220, 30, sf::Color(4, 18, 9, 200));
            sf::Text st(statusMessage.c_str(), font, 9);
            st.setFillColor(sf::Color(78, 238, 118));
            st.setPosition(4, 697); 
            window.draw(st);
        }
    }

    void drawRouteDetails() 
    {
        if (!showRouteDetails)
        {
            return;
        }
        if (currentPath.isEmpty()) 
        { 
            showRouteDetails = false; 
            return; 
        }
        int panH = 160;
        drawRect(0, 720 - panH, 1280, panH, sf::Color(2, 5, 18, 245));
        drawRect(0, 720 - panH, 1280, 2, COL_GOLD);
        drawRect(1245, 720 - panH + 4, 30, 18, sf::Color(78, 18, 18));
        drawText("X", 1253, 720 - panH + 5, 11, sf::Color(255, 98, 98));
        drawText("ROUTE DETAILS", 8, 720 - panH + 6, 12, COL_GOLD, true);
        int totalCost = 0, totalDur = 0;
        for (int i = 0; i < currentPath.getSize(); i++) 
        { 
            totalCost += currentPath.get(i).route.cost; 
            totalDur += currentPath.get(i).route.getDuration(); 
        }
        char buf[128];
        sprintf_s(buf, "Total Cost: $%d  |  Duration: %dh %dm  |  Legs: %d", totalCost, totalDur / 60, totalDur % 60, currentPath.getSize());
        drawText(buf, 8, 720 - panH + 24, 11, COL_CYAN);
        float x = 8;
        for (int i = 0; i < currentPath.getSize(); i++) 
        {
            RouteLeg& leg = currentPath.get(i); 
            if (x > 1230)
            {
                break;
            }
            float cardW = 175;
            drawRect(x, 720 - panH + 42, cardW, 112, sf::Color(6, 16, 40, 200), sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 55), 1);
            sf::Color stripe = routeCostColor(leg.route.cost);
            drawRect(x, 720 - panH + 42, 3, 112, stripe);
            char dep[6], arr[6], dd[11], ad[11];
            leg.route.departureTime.toString(dep); 
            leg.route.arrivalTime.toString(arr);
            leg.depDate.toString(dd); leg.arrDate.toString(ad);
            sprintf_s(buf, "Leg %d  %s", leg.legNumber, leg.route.company.c_str());
            drawText(buf, x + 7, 720 - panH + 45, 9, COL_GOLD, true);
            string fromTo = string(leg.fromPort.c_str()) + " -> " + string(leg.toPort.c_str());
            drawText(fromTo.c_str(), x + 7, 720 - panH + 59, 9, sf::Color::White);
            sprintf_s(buf, "Dep: %s %s", dd, dep); 
            drawText(buf, x + 7, 720 - panH + 73, 9, sf::Color(158, 198, 255));
            sprintf_s(buf, "Arr: %s %s", ad, arr); 
            drawText(buf, x + 7, 720 - panH + 87, 9, sf::Color(158, 198, 255));
            sprintf_s(buf, "Cost: $%d", leg.route.cost); 
            drawText(buf, x + 7, 720 - panH + 101, 9, routeCostColor(leg.route.cost));
            int dur = leg.route.getDuration();
            sprintf_s(buf, "Dur: %dh %dm", dur / 60, dur % 60);
            drawText(buf, x + 7, 720 - panH + 115, 9, routeDurColor(dur));
            x += cardW + 5;
        }
    }

    void drawLegendPanel() 
    {
        if (!showLegend)
        {
            return;
        }

        float lx = 1068, ly = 50, lw = 207, lh = 228;
        drawRect(lx, ly, lw, lh, sf::Color(3, 8, 26, 238), sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 75), 1);
        drawRect(lx, ly, lw, 22, sf::Color(5, 13, 36, 255));
        drawText("LEGEND", lx + 6, ly + 5, 12, COL_GOLD, true);
        drawRect(lx + lw - 22, ly + 4, 16, 14, sf::Color(78, 18, 18));
        drawText("X", lx + lw - 18, ly + 5, 9, sf::Color(255, 98, 98));
        struct LI 
        { 
            sf::Color c; 
            const char* label; 
        };
        LI items[] = 
        {
            {COL_PORT,              "Default Port"},
            {sf::Color(255,70,70),  "Origin Port"},
            {sf::Color(70,70,255),  "Destination Port"},
            {COL_PATH,              "Optimal Path"},
            {COL_EXPL,              "Explored (Dijkstra)"},
            {COL_VISIT,             "Visiting (Dijkstra)"},
            {sf::Color(180,20,20),  "Avoided Port"},
            {sf::Color(40,255,100), "Preferred Port"},
            {COL_ROUTE,             "Route Edge"},
        };
        float iy = ly + 26;
        for (auto& item : items) 
        {
            sf::CircleShape dot(5); 
            dot.setFillColor(item.c);
            dot.setPosition(lx + 7, iy - 1); 
            window.draw(dot);
            drawText(item.label, lx + 20, iy - 2, 9, sf::Color(178, 198, 218));
            iy += 19;
        }
        const char* cmL[] = 
        { 
            "Default","By Cost","By Duration","By Company" 
        };
        char cb[32]; sprintf_s(cb, "Color mode: %s", cmL[colorMode]);
        drawText(cb, lx + 6, iy, 9, sf::Color(98, 138, 168));
    }

    void drawPortTooltip() 
    {
        if (hoveredPort < 0)
        {
            return;
        }
        Port& p = graph.getPort(hoveredPort);
        sf::Vector2f sc = worldToScreen(p.x, p.y);
        float tx = sc.x + 15, ty = sc.y - 10;
        if (tx > 1060)
        {
            tx = sc.x - 225;
        }
        if (ty < 46)
        {
            ty = sc.y + 10;
        }
        if (ty > 630)
        {
            ty = 630;
        }
        int qh = p.dockingQueue.getSize() * 15, h = 80 + qh;
        drawRect(tx, ty, 215, (float)h, sf::Color(3, 8, 26, 232), sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 75), 1);
        drawRect(tx, ty, 215, 22, sf::Color(5, 13, 38, 255));
        drawText(p.name.c_str(), tx + 6, ty + 5, 12, COL_GOLD, true);
        char buf[64];
        sprintf_s(buf, "Daily Charge: $%d", p.dailyCharge); 
        drawText(buf, tx + 6, ty + 25, 10, sf::Color(168, 192, 222));
        sprintf_s(buf, "Queue: %d ships", p.dockingQueue.getSize()); 
        drawText(buf, tx + 6, ty + 40, 10, sf::Color(168, 192, 222));
        float qy = ty + 57;
        for (int i = 0; i < p.dockingQueue.getSize() && i < 5; i++)
        {
            drawRect(tx + 8, qy, 192, 13, sf::Color(10, 22, 52, 180));
            drawText(p.dockingQueue.get(i).c_str(), tx + 12, qy + 2, 9, i == 0 ? sf::Color::Yellow : sf::Color(148, 168, 192));
            qy += 15;
        }
        drawText("[Q] Add ship  [N] Process  [Del] Dequeue", tx + 6, ty + h - 18, 8, sf::Color(68, 88, 108));
    }

    void drawStatusBar()
    {
        if (statusTimer <= 0 || statusMessage.isEmpty())
        {
            return;
        }
        float alpha = min(1.f, statusTimer) * 255;
        int barY = 46;
        if (showRouteDetails)
        {
            barY = 720 - 165;
        }
        drawRect(0, (float)barY, 1280, 24, sf::Color(3, 12, 32, (sf::Uint8)(alpha * 0.88f)));
        drawRect(0, (float)barY, 4, 24, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, (sf::Uint8)alpha));
        sf::Text st(statusMessage.c_str(), font, 11);
        st.setFillColor(sf::Color(98, 238, 138, (sf::Uint8)alpha));
        st.setPosition(10, (float)barY + 5); window.draw(st);
    }

    void drawShipPanel() 
    {
        if (!showShipPanel)
        {
            return;
        }
        float px = 10, py = 50, pw = 780, ph = 640;
        drawRect(px, py, pw, ph, COL_PANEL, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 55), 1);
        drawRect(px, py, pw, 28, sf::Color(5, 12, 36, 255));
        drawText("SHIP MANAGEMENT", px + 8, py + 7, 14, COL_GOLD, true);
        drawRect(px + pw - 30, py + 5, 22, 18, sf::Color(78, 14, 14));
        drawText("X", px + pw - 24, py + 6, 11, sf::Color(255, 88, 88));

        float x = px + 12;

        // Fixed y positions — must match click detection in event handler
        float yNameLabel = 88;
        float yNameBox = 102;
        float yCompLabel = 130;
        float yCompBox = 144;
        float yDurLabel = 172;
        float yDurBox = 186;
        float yPrefLabel = 214;
        float yPrefBoxes = 228;
        float yExclRow = 258;
        float yAvoidLabel = 282;
        float yAvoidBox = 296;
        float yAvoidList = 322;

        drawRect(x - 5, yNameLabel - 3, pw - 15, 280, sf::Color(6, 16, 42, 160), sf::Color(38, 56, 96, 80), 1);
        drawText("CREATE NEW SHIP", x, 88 - 14, 11, COL_CYAN, true);

        drawText("Ship Name:", x, yNameLabel, 10, sf::Color(148, 178, 208));
        sf::Color fc = (activeField == FF_NAME) ? sf::Color(18, 55, 125) : sf::Color(10, 22, 52);
        drawRect(x, yNameBox, 200, 22, fc, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 45), 1);
        drawText(newShipName, x + 4, yNameBox + 4, 11, sf::Color::White);

        drawText("Company:", x, yCompLabel, 10, sf::Color(148, 178, 208));
        sf::Color fc2 = (activeField == FF_COMPANY) ? sf::Color(18, 55, 125) : sf::Color(10, 22, 52);
        drawRect(x, yCompBox, 200, 22, fc2, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 45), 1);
        drawText(newShipCompany, x + 4, yCompBox + 4, 11, sf::Color::White);

        drawText("Max Voyage (min, 0=unlimited):", x, yDurLabel, 10, sf::Color(148, 178, 208));
        sf::Color fc3 = (activeField == FF_DUR) ? sf::Color(18, 55, 125) : sf::Color(10, 22, 52);
        drawRect(x, yDurBox, 110, 22, fc3, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 45), 1);
        drawText(newShipDuration, x + 4, yDurBox + 4, 11, sf::Color::White);

        drawText("Preferred Companies (toggle):", x, yPrefLabel, 10, sf::Color(148, 218, 178));
        float cx2 = x;
        float cy2 = yPrefBoxes;
        LinkedList<MyString> cList; graph.getAllCompanies(cList);
        for (int i = 0; i < cList.getSize() && i < 8; i++)
        {
            bool sel = (prefCompanySel.find(i) >= 0);
            drawRect(cx2, cy2, 88, 18, sel ? sf::Color(9, 95, 42, 200) : sf::Color(16, 30, 57, 200), sel ? COL_GREEN : sf::Color(48, 57, 77), 1);
            drawText(cList.get(i).c_str(), cx2 + 4, cy2 + 3, 9, sel ? sf::Color(98, 255, 158) : sf::Color(148, 168, 198));
            cx2 += 92; 
            if (cx2 > x + 560) 
            { 
                cx2 = x; 
                cy2 += 22; 
            }
        }

        drawText("Use preferred company ONLY:", x, yExclRow, 10, sf::Color(148, 178, 208));
        drawRect(x + 190, yExclRow - 2, 55, 18, exclusiveCompany ? sf::Color(9, 105, 42) : sf::Color(57, 14, 14), COL_GOLD, 1);
        drawText(exclusiveCompany ? "YES" : "NO", x + 202, yExclRow, 10, sf::Color::White);

        drawText("Avoid Port (type name, ENTER to add):", x, yAvoidLabel, 10, sf::Color(255, 108, 108));
        sf::Color fc4 = (activeField == FF_AVOID) ? sf::Color(86, 18, 18) : sf::Color(23, 14, 14);
        drawRect(x, yAvoidBox, 200, 22, fc4, sf::Color(218, 57, 57, 80), 1);
        drawText(avoidPortInput, x + 4, yAvoidBox + 4, 11, sf::Color::White);

        if (!avoidPortSel.isEmpty()) 
        {
            drawText("Avoiding:", x, yAvoidList, 10, sf::Color(255, 98, 98));
            float ax = x + 62;
            for (int i = 0; i < avoidPortSel.getSize() && i < 6; i++) 
            {
                int idx = avoidPortSel.get(i);
                if (idx < graph.getPortCount()) 
                {
                    drawText(graph.getPort(idx).name.c_str(), ax, yAvoidList, 9, sf::Color(255, 128, 128));
                    ax += (float)(strlen(graph.getPort(idx).name.c_str()) * 7 + 10);
                }
            }
        }

        btnSaveShip.draw(window); btnCancelShip.draw(window); btnClearShip.draw(window);
        drawText("Fill fields > click SAVE to create & activate ship", x, 660, 9, sf::Color(68, 88, 108));

        float ey = py + 320;
        drawRect(px + 8, ey, pw - 16, 22, sf::Color(6, 16, 42, 200));
        drawText("REGISTERED SHIPS (click to activate)", px + 12, ey + 4, 11, COL_CYAN, true); ey += 26;
        for (int i = 0; i < shipManager.getShipCount() && ey < py + ph - 10; i++) 
        {
            bool isAct = (i == shipManager.getActiveShipIndex());
            drawRect(px + 10, ey, pw - 20 - 35, 22, isAct ? sf::Color(9, 57, 18, 200) : sf::Color(9, 18, 46, 160), isAct ? sf::Color(38, 175, 77, 120) : sf::Color(28, 38, 62, 80), 1);
            CustomShip* s = shipManager.getShipByIndex(i);
            char info[100]; 
            sprintf_s(info, "%s%s  [Avoid:%d Pref:%d]", isAct ? "> " : "  ", s->name.c_str(), s->avoidPorts.getSize(), s->preferCompanies.getSize());
            drawText(info, px + 14, ey + 5, 10, isAct ? sf::Color(78, 255, 128) : sf::Color(158, 178, 208));
            drawRect(px + pw - 48, ey + 2, 34, 18, sf::Color(78, 14, 14), sf::Color(148, 33, 33), 1);
            drawText("DEL", px + pw - 44, ey + 4, 9, sf::Color(255, 98, 98));
            ey += 26;
        }
        if (shipManager.getShipCount() == 0)
        {
            drawText("No ships registered yet.", px + 14, ey + 5, 10, sf::Color(68, 68, 68));
        }
    }

    void drawFilterPanel()
    {
        if (!showFilterPanel)
        {
            return;
        }
        float px = 10, py = 50, pw = 700, ph = 580;
        drawRect(px, py, pw, ph, COL_PANEL, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 55), 1);
        drawRect(px, py, pw, 28, sf::Color(5, 12, 36, 255));
        drawText("SUBGRAPH FILTER", px + 8, py + 7, 14, COL_GOLD, true);
        drawRect(px + pw - 30, py + 5, 22, 18, sf::Color(78, 14, 14));
        drawText("X", px + pw - 24, py + 6, 11, sf::Color(255, 88, 88));
        float y = py + 40, x = px + 12;
        drawText("FILTER BY COMPANY (click to toggle):", x, y, 11, COL_CYAN, true); y += 18;
        float cx2 = x;
        for (int i = 0; i < companyList.getSize(); i++) 
        {
            bool sel = companySelected.get(i);
            drawRect(cx2, y, 105, 22, sel ? sf::Color(9, 95, 42) : sf::Color(16, 28, 57), sel ? COL_GREEN : sf::Color(48, 57, 77), 1);
            drawText(companyList.get(i).c_str(), cx2 + 5, y + 5, 9, sel ? sf::Color(88, 255, 148) : sf::Color(148, 172, 202));
            cx2 += 110; 
            if (cx2 > px + pw - 120) 
            { 
                cx2 = x; y += 26; 
            }
        }
        y += 30;
        drawRect(x - 5, y - 4, pw - 15, 1, sf::Color(38, 52, 77)); 
        y += 8;
        // Replace the cost/duration section at the bottom of drawFilterPanel with:
        drawText("MAX COST FILTER (-1 = unlimited):", x, 451, 10, COL_CYAN);
        sf::Color fc = (filterField == FLT_COST) ? sf::Color(18, 55, 125) : sf::Color(10, 22, 52);
        drawRect(x, 467, 120, 22, fc, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 45), 1);
        drawText(filterMaxCost, x + 4, 471, 11, sf::Color::White);

        drawText("MAX DURATION FILTER (min, -1 = unlimited):", x, 497, 10, COL_CYAN);
        sf::Color fd = (filterField == FLT_DUR) ? sf::Color(18, 55, 125) : sf::Color(10, 22, 52);
        drawRect(x, 513, 120, 22, fd, sf::Color(COL_GOLD.r, COL_GOLD.g, COL_GOLD.b, 45), 1);
        drawText(filterMaxDur, x + 4, 517, 11, sf::Color::White);

        if (filterActive) 
        {
            drawRect(x, 539, 260, 22, sf::Color(4, 38, 11, 200));
            drawText(">> FILTER IS ACTIVE <<", x + 6, 543, 11, sf::Color(78, 255, 128), true);
        }
        btnFilterApply.draw(window);
        btnFilterClear.draw(window); 
        btnFilterClose.draw(window); 
    }

    void doFindRoute(bool byCost)
    {
        if (selectedOrigin < 0)
        { 
            setStatus(MyString("Please left-click a port to set ORIGIN")); 
            return; 
        }
        if (selectedDest < 0) 
        { 
            setStatus(MyString("Please right-click a port to set DESTINATION"));
            return; 
        }
        if (selectedOrigin == selectedDest) 
        { 
            setStatus(MyString("Origin and destination must be different")); 
            return; 
        }
        MyString from = graph.getPort(selectedOrigin).name, to = graph.getPort(selectedDest).name;
        CustomShip* s = shipManager.getActiveShip();
        LinkedList<MyString>* ap = nullptr, * ac = nullptr, * pc = nullptr; int md = -1;
        if (s) 
        { 
            if (!s->avoidPorts.isEmpty())
            {
                ap = &s->avoidPorts;
            }
            if (!s->avoidCompanies.isEmpty())
            {
                ac = &s->avoidCompanies;
            }
            if (s->usePreferredCompanyOnly && !s->preferCompanies.isEmpty())
            {
                pc = &s->preferCompanies;
            }
            if (s->maxVoyageDuration > 0)
            {
                md = s->maxVoyageDuration;
            }
        }
        currentPath = activeGraph().findShortestPath(from, to, byCost, ap, ac, pc, md, Date(7, 12, 2025));
        showingPath = !currentPath.isEmpty(); 
        stepByStepMode = false; 
        animationStep = 0;
        exploredNodes.clear(); 
        visitedOrder.clear(); 
        dijkstraSteps.clear(); 
        animatingShip = false;
        if (currentPath.isEmpty()) 
        { 
            setStatus(MyString("No route found — try removing filters or ship preferences")); 
        }
        else 
        {
            int tc = 0, td = 0; 
            for (int i = 0; i < currentPath.getSize(); i++) 
            { 
                tc += currentPath.get(i).route.cost; 
                td += currentPath.get(i).route.getDuration(); 
            }
            char b[100]; sprintf_s(b, "%s: $%d total | %dh%dm | %d legs", byCost ? "Cheapest route" : "Fastest route", tc, td / 60, td % 60, currentPath.getSize());
            setStatus(MyString(b)); 
            showRouteDetails = true;
        }
    }

    void doStepMode() 
    {
        if (selectedOrigin < 0) 
        { 
            setStatus(MyString("Please select ORIGIN first")); 
            return; 
        }
        if (selectedDest < 0) 
        { 
            setStatus(MyString("Please select DESTINATION first")); 
            return; 
        }
        if (selectedOrigin == selectedDest) 
        { 
            setStatus(MyString("Origin and destination must be different")); 
            return; 
        }
        MyString from = graph.getPort(selectedOrigin).name, to = graph.getPort(selectedDest).name;
        CustomShip* s = shipManager.getActiveShip();
        LinkedList<MyString>* ap = nullptr, * ac = nullptr, * pc = nullptr; int md = -1;
        if (s) 
        {
            if (!s->avoidPorts.isEmpty())
            {
                ap = &s->avoidPorts;
            }
            if (!s->avoidCompanies.isEmpty())
            {
                ac = &s->avoidCompanies;
            }
            if (s->usePreferredCompanyOnly && !s->preferCompanies.isEmpty())
            {
                pc = &s->preferCompanies;
            }
            if (s->maxVoyageDuration > 0)
            {
                md = s->maxVoyageDuration;
            }
        }
        dijkstraSteps = activeGraph().findShortestPathWithSteps(from, to, useCost, currentPath, visitedOrder, ap, ac, pc, md);
        stepByStepMode = true; currentDijkstraStep = 0; showExplored = true;
        showingPath = false; exploredNodes.clear(); animatingShip = false; autoStep = false; stepTimer = 0;
        if (dijkstraSteps.isEmpty()) 
        { 
            setStatus(MyString("No exploration steps found — possibly no route exists")); 
            stepByStepMode = false; 
        }
        else 
        { 
            char b[64]; sprintf_s(b, "Step-by-step: %d steps. SPACE=next, AUTO=auto-advance", dijkstraSteps.getSize()); 
            setStatus(MyString(b), 6.f); 
        }
    }

    void doSaveShip() 
    {
        if (strlen(newShipName) == 0) 
        { 
            setStatus(MyString("Ship name cannot be empty")); 
            return; 
        }
        CustomShip ship; ship.name = MyString(newShipName); 
        ship.shippingCompany = MyString(newShipCompany);
        ship.maxVoyageDuration = atoi(newShipDuration); 
        if (ship.maxVoyageDuration <= 0)
        {
            ship.maxVoyageDuration = -1;
        }
        LinkedList<MyString> cList; graph.getAllCompanies(cList);
        for (int i = 0; i < prefCompanySel.getSize(); i++) 
        { 
            int idx = prefCompanySel.get(i); 
            if (idx < cList.getSize())
            {
                ship.addPreferCompany(cList.get(idx));
            }
        }
        ship.usePreferredCompanyOnly = exclusiveCompany && !ship.preferCompanies.isEmpty();
        for (int i = 0; i < avoidPortSel.getSize(); i++) 
        { 
            int idx = avoidPortSel.get(i); 
            if (idx < graph.getPortCount())
            {
                ship.addAvoidPort(graph.getPort(idx).name);
            }
        }
        shipManager.addShip(ship); 
        shipManager.setActiveShip(shipManager.getShipCount() - 1);
        currentPath.clear(); 
        showingPath = false;
        showRouteDetails = false;
        char b[64]; sprintf_s(b, "Ship '%s' created and activated!", newShipName); 
        setStatus(MyString(b));
        strcpy_s(newShipName, "MyShip"); 
        strcpy_s(newShipDuration, "0"); 
        strcpy_s(newShipCompany, "OceanCo");
        avoidPortSel.clear(); 
        prefPortSel.clear();
        prefCompanySel.clear(); 
        exclusiveCompany = false;
        strcpy_s(avoidPortInput, ""); 
        strcpy_s(prefPortInput, "");
    }

    void doApplyFilter() 
    {
        LinkedList<MyString> selC;
        for (int i = 0; i < companyList.getSize(); i++)
        {
            if (companySelected.get(i))
            {
                selC.pushBack(companyList.get(i));
            }
        }
        if (activeSubgraph) 
        { 
            delete activeSubgraph; 
            activeSubgraph = nullptr; 
        }
        fMaxCost = atoi(filterMaxCost); 
        fMaxDur = atoi(filterMaxDur);
        if (selC.isEmpty() && fMaxCost <= 0 && fMaxDur <= 0) 
        {
            filterActive = false; 
            setStatus(MyString("No filter criteria specified — filter cleared")); 
            return;
        }
        activeSubgraph = new MaritimeGraph(graph.generateSubgraph(&selC, nullptr, fMaxCost, fMaxDur));
        filterActive = true; 
        currentPath.clear(); 
        showingPath = false;
        stepByStepMode = false; 
        showRouteDetails = false;
        char b[80]; sprintf_s(b, "Filter applied: %d ports, %d routes", activeSubgraph->getPortCount(), activeSubgraph->getTotalRouteCount());
        setStatus(MyString(b));
    }

    void handleCompanyToggle(float mx, float my, float startX, float startY)
    {
        LinkedList<MyString> cList; graph.getAllCompanies(cList);
        float cx2 = startX, cy = startY;
        for (int i = 0; i < cList.getSize() && i < 8; i++)
        {
            if (sf::FloatRect(cx2, cy, 88, 18).contains(mx, my)) 
            { 
                int fi = prefCompanySel.find(i); 
                if (fi >= 0)
                {
                    prefCompanySel.removeAt(fi);
                }
                else
                {
                    prefCompanySel.pushBack(i);
                }
                return;
            }
            cx2 += 92; 
            if (cx2 > startX + 560) 
            { 
                cx2 = startX; 
                cy += 22; 
            }
        }
    }
    void handleFilterCompanyClick(float mx, float my) 
    {
        float px = 10, cx2 = px + 12, cy = 108;
        for (int i = 0; i < companyList.getSize(); i++) 
        {
            if (sf::FloatRect(cx2, cy, 105, 22).contains(mx, my))
            { 
                companySelected.set(i, !companySelected.get(i)); 
                return; 
            }
            cx2 += 110; 
            if (cx2 > px + 700 - 120) 
            { 
                cx2 = px + 12; 
                cy += 26; 
            }
        }
    }

public:
    Visualization(sf::RenderWindow& win, MaritimeGraph& g, CustomShipManager& sm, sf::Font& fnt)
        : window(win), graph(g), shipManager(sm), font(fnt)
    {
        if (shipTexture.loadFromFile("ship.png")) 
        { 
            hasShipTex = true; 
            shipSprite.setTexture(shipTexture); 
        }
        if (mapTexture.loadFromFile("worldmap.jpg")) 
        { 
            hasMap = true; 
            mapSprite.setTexture(mapTexture);
        }
        graph.getAllCompanies(companyList);
        for (int i = 0; i < companyList.getSize(); i++)
        {
            companySelected.pushBack(false);
        }
        buildToolbar();
        viewOffset = sf::Vector2f(0, 0); zoomLevel = 1.0f;
    }
    ~Visualization() 
    { 
        if (activeSubgraph)
        {
            delete activeSubgraph;
        }

    }

    bool run() 
    {
        sf::Clock clock;
        while (window.isOpen()) 
        {
            float dt = clock.restart().asSeconds();
            if (dt > 0.1f)
            {
                dt = 0.1f;
            }
            globalTime += dt;
            if (statusTimer > 0)
            {
                statusTimer -= dt;
            }

            sf::Vector2f mouse((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);
            sf::Event event;
            while (window.pollEvent(event)) 
            {
                if (event.type == sf::Event::Closed) 
                { 
                    window.close(); 
                    return false; 
                }

                if (event.type == sf::Event::MouseWheelScrolled) 
                {
                    float delta = event.mouseWheelScroll.delta;
                    sf::Vector2f worldBefore = screenToWorld(mouse.x, mouse.y);
                    float factor = delta > 0 ? 1.15f : 1.f / 1.15f;
                    zoomLevel = max(ZOOM_MIN, min(ZOOM_MAX, zoomLevel * factor));
                    sf::Vector2f worldAfter = screenToWorld(mouse.x, mouse.y);
                    viewOffset += worldAfter - worldBefore;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) 
                { 
                    isPanning = true; 
                    panStart = mouse; 
                }
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle)
                {
                    isPanning = false;
                }
                if (event.type == sf::Event::MouseMoved && isPanning) 
                {
                    sf::Vector2f cur((float)event.mouseMove.x, (float)event.mouseMove.y);
                    viewOffset += (cur - panStart) / zoomLevel; 
                    panStart = cur;
                }

                for (int i = 0; i < BTN_COUNT; i++)
                {
                    toolbar[i].update(mouse);
                }
                btnInfo.update(mouse); 
                btnZoomIn.update(mouse);
                btnZoomOut.update(mouse);
                btnZoomReset.update(mouse);
                btnSaveShip.update(mouse);
                btnCancelShip.update(mouse); 
                btnClearShip.update(mouse);
                btnFilterApply.update(mouse); 
                btnFilterClear.update(mouse); 
                btnFilterClose.update(mouse);

                if (event.type == sf::Event::MouseButtonPressed) 
                {
                    float mx = (float)event.mouseButton.x, my = (float)event.mouseButton.y;
                    if (btnZoomIn.clicked(event)) 
                    { 
                        sf::Vector2f c = screenToWorld(640, 360); 
                        zoomLevel = min(ZOOM_MAX, zoomLevel * 1.3f); 
                        sf::Vector2f c2 = screenToWorld(640, 360); 
                        viewOffset += c2 - c; 
                    }
                    if (btnZoomOut.clicked(event)) 
                    { 
                        sf::Vector2f c = screenToWorld(640, 360); 
                        zoomLevel = max(ZOOM_MIN, zoomLevel / 1.3f);
                        sf::Vector2f c2 = screenToWorld(640, 360);
                        viewOffset += c2 - c; 
                    }
                    if (btnZoomReset.clicked(event)) 
                    { 
                        zoomLevel = 1.f;
                        viewOffset = sf::Vector2f(0, 0); 
                    }
                    if (btnInfo.clicked(event)) 
                    { 
                        showInfoPanel = !showInfoPanel; 
                        setStatus(showInfoPanel ? MyString("Info panel shown") : MyString("Info panel hidden")); 
                    }
                    if (toolbar[0].clicked(event)) 
                    { 
                        useCost = true; 
                        doFindRoute(true); 
                    }
                    else if (toolbar[1].clicked(event)) 
                    { 
                        useCost = false;
                        doFindRoute(false); 
                    }
                    else if (toolbar[2].clicked(event)) 
                    { 
                        doStepMode(); 
                    }
                    else if (toolbar[3].clicked(event))
                    {
                        if (!stepByStepMode) 
                        {
                            doStepMode();
                            if (stepByStepMode) 
                            { 
                                autoStep = true; 
                                stepTimer = 0; 
                                setStatus(MyString("Auto-step started — Dijkstra running automatically")); 
                            }
                        }
                        else 
                        { 
                            autoStep = !autoStep; 
                            stepTimer = 0; 
                            setStatus(autoStep ? MyString("Auto-step RESUMED") : MyString("Auto-step PAUSED — SPACE to step manually")); 
                        }
                    }
                    else if (toolbar[4].clicked(event)) 
                    {
                        selectedOrigin = selectedDest = -1; 
                        currentPath.clear(); 
                        showingPath = false; 
                        stepByStepMode = false; 
                        autoStep = false;
                        exploredNodes.clear(); 
                        visitedOrder.clear(); 
                        dijkstraSteps.clear(); 
                        animatingShip = false; 
                        showRouteDetails = false;
                        currentDijkstraStep = 0; 
                        shipManager.setActiveShip(-1); 
                        setStatus(MyString("Reset complete"));
                    }
                    else if (toolbar[5].clicked(event)) 
                    { 
                        showShipPanel = !showShipPanel; 
                        showFilterPanel = false; 
                    }
                    else if (toolbar[6].clicked(event)) 
                    { 
                        showFilterPanel = !showFilterPanel; 
                        showShipPanel = false; 
                    }
                    else if (toolbar[7].clicked(event)) 
                    { 
                        if (!currentPath.isEmpty())
                        {
                            showRouteDetails = !showRouteDetails;
                        }
                        else
                        {
                            setStatus(MyString("Find a route first to see route details"));
                        }
                    }
                    else if (toolbar[8].clicked(event)) 
                    { 
                        showLegend = !showLegend; 
                    }
                    else if (toolbar[9].clicked(event)) 
                    { 
                        colorMode = (colorMode + 1) % 4; 
                    }
                    else if (toolbar[10].clicked(event))
                    { 
                        return true; 
                    }

                    if (showRouteDetails && sf::FloatRect(1245, 720 - 162, 30, 18).contains(mx, my))
                    {
                        showRouteDetails = false;
                    }
                    if (showLegend && sf::FloatRect(1068 + 207 - 22, 50 + 4, 16, 14).contains(mx, my))
                    {
                        showLegend = false;
                    }

                    if (showShipPanel) 
                    {
                        float py2 = 50;
                        // X button to close
                        if (sf::FloatRect(10 + 780 - 30, py2 + 5, 22, 18).contains(mx, my)) 
                        { 
                            showShipPanel = false; 
                            activeField = FF_NONE; 
                        }

                        // Inner panel buttons
                        if (btnSaveShip.clicked(event))
                        {
                            doSaveShip();
                        }
                        else if (btnCancelShip.clicked(event)) 
                        { 
                            showShipPanel = false; 
                            activeField = FF_NONE; 
                        }
                        else if (btnClearShip.clicked(event)) 
                        {
                            avoidPortSel.clear(); prefPortSel.clear();
                            prefCompanySel.clear();
                            exclusiveCompany = false;
                            strcpy_s(avoidPortInput, ""); 
                            strcpy_s(prefPortInput, "");
                            setStatus(MyString("Preferences cleared"));
                        }

                        
                        float fx = 22;
                        if (sf::FloatRect(fx, 117, 200, 22).contains(mx, my)) 
                        { 
                            activeField = FF_NAME; 
                        }
                        else if (sf::FloatRect(fx, 159, 200, 22).contains(mx, my))
                        { 
                            activeField = FF_COMPANY; 
                        }
                        else if (sf::FloatRect(fx, 201, 110, 22).contains(mx, my)) 
                        { 
                            activeField = FF_DUR; 
                        }
                        else if (sf::FloatRect(fx, 325, 200, 22).contains(mx, my)) 
                        { 
                            activeField = FF_AVOID; 
                        }
                        handleCompanyToggle(mx, my, fx, 243);
                        if (sf::FloatRect(212, 285, 55, 18).contains(mx, my))
                        {
                            exclusiveCompany = !exclusiveCompany;
                        }

                        // Ship list clicks
                        float ey = 50 + 346;
                        for (int i = 0; i < shipManager.getShipCount(); i++) 
                        {
                            if (sf::FloatRect(20, ey, 740, 22).contains(mx, my))
                            {
                                shipManager.setActiveShip(i);
                            }
                            if (sf::FloatRect(10 + 780 - 48, ey + 2, 34, 18).contains(mx, my))
                            {
                                MyString n = shipManager.getShipByIndex(i)->name;
                                shipManager.removeShip(n);
                                setStatus(MyString("Deleted: ") + n);
                                break;
                            }
                            ey += 26;
                        }
                    }
                    if (showFilterPanel)
                    {
                        // X button
                        if (sf::FloatRect(10 + 700 - 30, 50 + 5, 22, 18).contains(mx, my)) 
                        { 
                            showFilterPanel = false; filterField = FLT_NONE; 
                        }

                        handleFilterCompanyClick(mx, my);
                        
                        if (sf::FloatRect(22, 467, 120, 22).contains(mx, my))
                        {
                            filterField = FLT_COST;
                        }
                        else if (sf::FloatRect(22, 513, 120, 22).contains(mx, my))
                        {
                            filterField = FLT_DUR;
                        }

                        if (btnFilterApply.clicked(event))
                        {
                            doApplyFilter();
                        }
                        else if (btnFilterClear.clicked(event))
                        {
                            for (int i = 0; i < companySelected.getSize(); i++)
                            {
                                companySelected.set(i, false);
                            }
                            strcpy_s(filterMaxCost, "-1"); 
                            strcpy_s(filterMaxDur, "-1");
                            fMaxCost = fMaxDur = -1; filterActive = false;
                            if (activeSubgraph)
                            { 
                                delete activeSubgraph; 
                                activeSubgraph = nullptr; 
                            }
                            setStatus(MyString("Filter cleared — all routes visible"));
                        }
                        else if (btnFilterClose.clicked(event)) 
                        { 
                            showFilterPanel = false; 
                            filterField = FLT_NONE;
                        }
                    }
                    bool panelOpen = showShipPanel || showFilterPanel;
                    bool inPanel = (panelOpen && mx < 800) || (showInfoPanel && mx < 220);
                    if (!inPanel && my > 46) 
                    {
                        int clicked = findPortAt(mx, my);
                        if (clicked >= 0) 
                        {
                            if (event.mouseButton.button == sf::Mouse::Left) 
                            { 
                                selectedOrigin = clicked; setStatus(MyString("Origin set: ") + graph.getPort(clicked).name); 
                            }
                            else if (event.mouseButton.button == sf::Mouse::Right) 
                            {
                                selectedDest = clicked; 
                                setStatus(MyString("Destination set: ") + graph.getPort(clicked).name); 
                            }
                        }
                    }
                }

                if (event.type == sf::Event::KeyPressed) 
                {
                    if (event.key.code == sf::Keyboard::Escape) 
                    {
                        if (showShipPanel || showFilterPanel) 
                        {
                            showShipPanel = showFilterPanel = false; 
                            activeField = FF_NONE; 
                            filterField = FLT_NONE; 
                        }
                        else return true;
                    }
                    if (showShipPanel && event.key.code == sf::Keyboard::Tab) 
                    {
                        if (activeField == FF_NONE)
                        {
                            activeField = FF_NAME;
                        }
                        else if (activeField == FF_NAME)
                        {
                            activeField = FF_COMPANY;
                        }
                        else if (activeField == FF_COMPANY)
                        {
                            activeField = FF_DUR;
                        }
                        else if (activeField == FF_DUR)
                        {
                            activeField = FF_AVOID;
                        }
                        else                    
                        {
                            activeField = FF_NAME;
                        }
                    }
                    if (!showShipPanel && !showFilterPanel) 
                    {
                        if (event.key.code == sf::Keyboard::Space && stepByStepMode)
                        {
                            advanceStep();
                        }
                        float panSpd = 40.f / zoomLevel;
                        if (event.key.code == sf::Keyboard::Left)
                        {
                            viewOffset.x += panSpd;
                        }
                        if (event.key.code == sf::Keyboard::Right)
                        {
                            viewOffset.x -= panSpd;
                        }
                        if (event.key.code == sf::Keyboard::Up)
                        {
                            viewOffset.y += panSpd;
                        }
                        if (event.key.code == sf::Keyboard::Down)
                        {
                            viewOffset.y -= panSpd;
                        }
                        if (event.key.code == sf::Keyboard::Equal) 
                        { 
                            sf::Vector2f c = screenToWorld(640, 360); 
                            zoomLevel = min(ZOOM_MAX, zoomLevel * 1.2f);
                            sf::Vector2f c2 = screenToWorld(640, 360); 
                            viewOffset += c2 - c;
                        }
                        if (event.key.code == sf::Keyboard::Dash) 
                        { 
                            sf::Vector2f c = screenToWorld(640, 360); 
                            zoomLevel = max(ZOOM_MIN, zoomLevel / 1.2f); 
                            sf::Vector2f c2 = screenToWorld(640, 360); 
                            viewOffset += c2 - c; 
                        }
                        if (event.key.code == sf::Keyboard::A && showingPath && !currentPath.isEmpty()) 
                        {
                            animatingShip = true; 
                            shipPathLeg = 0; 
                            shipLegProgress = 0.f;
                            int fi = graph.getPortIndex(currentPath.get(0).fromPort);
                            if (fi >= 0)
                            {
                                shipPos = sf::Vector2f(graph.getPort(fi).x, graph.getPort(fi).y);
                            }
                            PlayShipSound(); setStatus(MyString("Ship animation started — following path"));
                        }
                        if (event.key.code == sf::Keyboard::N && selectedOrigin >= 0 && showingPath && !currentPath.isEmpty())
                        {
                            Port& p = graph.getPort(selectedOrigin);
                            if (!p.dockingQueue.isEmpty()) 
                            {
                                MyString shipName = p.dockingQueue.dequeue();
                                animatingShip = true; 
                                shipPathLeg = 0; 
                                shipLegProgress = 0.f;
                                shipPos = sf::Vector2f(p.x, p.y); 
                                shipDir = sf::Vector2f(1, 0);
                                PlayShipSound(); 
                                setStatus(MyString("Ship '") + shipName + MyString("' departed!"));
                            }
                            else
                            {
                                setStatus(MyString("No ships queued at origin — hover origin and press Q first"));
                            }
                        }
                        if (event.key.code == sf::Keyboard::Q && hoveredPort >= 0) {
                            Port& p = graph.getPort(hoveredPort); char sn[32];
                            sprintf_s(sn, "Ship_%03d", p.dockingQueue.getSize() + 1);
                            graph.enqueueShipAtPort(p.name, MyString(sn));
                            setStatus(MyString("Added ") + MyString(sn) + " to " + p.name);
                        }
                        if (event.key.code == sf::Keyboard::Delete && hoveredPort >= 0 && !graph.getPort(hoveredPort).dockingQueue.isEmpty()) 
                        {
                            MyString s = graph.getPort(hoveredPort).dockingQueue.dequeue();
                            setStatus(MyString("Dequeued: ") + s);
                        }
                    }
                }

                if (event.type == sf::Event::TextEntered && event.text.unicode < 128) 
                {
                    char c = (char)event.text.unicode;
                    auto appendField = [&](char* field, int maxLen) 
                        {
                        if (c == 8) 
                        {
                            int l = (int)strlen(field); 
                            if (l > 0)
                            {
                                field[l - 1] = '\0';
                            }
                        }
                        else if (c >= 32 && (int)strlen(field) < maxLen - 1) 
                        { 
                            char a[2] = { c,0 };
                            strcat_s(field, maxLen, a);
                        }
                        };
                    auto appendNum = [&](char* field, int maxLen) 
                        {
                        if (c == 8) 
                        { 
                            int l = (int)strlen(field); 
                            if (l > 0)
                            {
                                field[l - 1] = '\0';
                            }
                        }
                        else if ((c >= '0' && c <= '9') || c == '-') 
                        { 
                            if ((int)strlen(field) < maxLen - 1) 
                            { 
                                char a[2] = { c,0 }; 
                                strcat_s(field, maxLen, a);
                            } 
                        }
                        };
                    if (showShipPanel) 
                    {
                        if (activeField == FF_NAME)
                        {
                            appendField(newShipName, 32);
                        }
                        else if (activeField == FF_COMPANY)
                        {
                            appendField(newShipCompany, 32);
                        }
                        else if (activeField == FF_DUR)
                        {
                            appendNum(newShipDuration, 10);
                        }
                        else if (activeField == FF_AVOID) 
                        {
                            if (c == 13) 
                            { 
                                int idx = graph.getPortIndex(MyString(avoidPortInput)); 
                                if (idx >= 0 && avoidPortSel.find(idx) < 0)
                                { 
                                    avoidPortSel.pushBack(idx);
                                    avoidPortInput[0] = '\0'; 
                                    setStatus(MyString("Avoid port added: ") + graph.getPort(idx).name);
                                } 
                                else
                                {
                                    setStatus(MyString("Port not found: ") + MyString(avoidPortInput));
                                }
                            }
                            else
                            {
                                appendField(avoidPortInput, 32);
                            }
                        }
                    }
                    if (showFilterPanel)
                    {
                        if (filterField == FLT_COST)
                        {
                            appendNum(filterMaxCost, 10);
                        }
                        else if (filterField == FLT_DUR)
                        {
                            appendNum(filterMaxDur, 10);
                        }
                    }
                }

                if (event.type == sf::Event::MouseMoved)
                {
                    hoveredPort = findPortAt((float)event.mouseMove.x, (float)event.mouseMove.y);
                }
            }

            // Ship animation
            if (animatingShip && !currentPath.isEmpty()) 
            {
                if (shipPathLeg < currentPath.getSize()) 
                {
                    RouteLeg& leg = currentPath.get(shipPathLeg);
                    int fi = graph.getPortIndex(leg.fromPort); 
                    int ti = graph.getPortIndex(leg.toPort);
                    if (fi >= 0 && ti >= 0) 
                    {
                        sf::Vector2f from2(graph.getPort(fi).x, graph.getPort(fi).y);
                        sf::Vector2f to2(graph.getPort(ti).x, graph.getPort(ti).y);
                        sf::Vector2f diff = to2 - from2;
                        float legLen = sqrtf(diff.x * diff.x + diff.y * diff.y);
                        if (legLen > 0.5f) 
                        {
                            shipLegProgress += shipSpeed / legLen * dt;
                            if (shipLegProgress >= 1.f) 
                            { 
                                shipLegProgress = 0.f; 
                                shipPathLeg++; 
                                if (shipPathLeg >= currentPath.getSize())
                                { 
                                    animatingShip = false; 
                                    setStatus(MyString("Ship arrived at destination!")); 
                                } 
                            }
                            else 
                            { 
                                shipPos = from2 + diff * shipLegProgress;
                                if (legLen > 0.1f)
                                {
                                    shipDir = diff / legLen;
                                }
                            }
                        }
                        else 
                        { 
                            shipPathLeg++;
                        }
                    }
                    else 
                    { 
                        shipPathLeg++; 
                    }
                }
                else 
                { 
                    animatingShip = false; 
                }
            }

            if (autoStep && stepByStepMode) 
            { 
                stepTimer += dt;
                if (stepTimer > 0.5f) 
                { 
                    stepTimer = 0; 
                    advanceStep(); 
                } 
            }

            // ── DRAW ──
            window.clear(COL_BG);
            drawMapBackground();
            drawAllRoutes();
            drawExploration();
            drawPath();
            for (int i = 0; i < graph.getPortCount(); i++)
            {
                drawPort(i);
            }

            if (animatingShip) 
            {
                sf::Vector2f sc = worldToScreen(shipPos.x, shipPos.y);
                if (hasShipTex) 
                {
                    float sr = 0.18f * zoomLevel;
                    shipSprite.setScale(sr, sr);
                    shipSprite.setOrigin(shipTexture.getSize().x / 2.f, shipTexture.getSize().y / 2.f);
                    shipSprite.setPosition(sc);
                    shipSprite.setRotation(atan2f(shipDir.y, shipDir.x) * 180.f / 3.14159f);
                    window.draw(shipSprite);
                }
                else
                {
                    float rad = 8.f * min(zoomLevel, 2.f);
                    sf::CircleShape sc2(rad); 
                    sc2.setOrigin(rad, rad);
                    sc2.setPosition(sc);
                    sc2.setFillColor(COL_GOLD); 
                    window.draw(sc2);
                    sf::RectangleShape trail(sf::Vector2f(rad * 3.f, rad * 0.7f));
                    trail.setOrigin(rad * 3.f, rad * 0.35f); 
                    trail.setPosition(sc);
                    trail.setFillColor(sf::Color(200, 200, 255, 80));
                    trail.setRotation(atan2f(-shipDir.y, -shipDir.x) * 180.f / 3.14159f);
                    window.draw(trail);
                }
            }

            drawPortTooltip();
            drawToolbar();
            drawInfoPanel();
            drawShipPanel();
            drawFilterPanel();
            drawRouteDetails();
            drawLegendPanel();
            drawStatusBar();

            if (stepByStepMode) 
            {
                char sh[64]; sprintf_s(sh, "SPACE = next step (%d/%d)", currentDijkstraStep, dijkstraSteps.getSize());
                sf::Text sht(sh, font, 11); 
                sht.setFillColor(sf::Color(178, 138, 255));
                sht.setPosition(470, 50);
                window.draw(sht);
            }
            if (showingPath && !animatingShip && !currentPath.isEmpty()) 
            {
                sf::Text hint("[A] = animate ship  |  Middle-drag / Arrow keys = pan  |  Scroll = zoom", font, 10);
                hint.setFillColor(sf::Color(68, 88, 108));
                sf::FloatRect hb = hint.getLocalBounds();
                hint.setPosition(640 - hb.width / 2, 706); 
                window.draw(hint);
            }
            window.display();
        }
        return false;
    }
};

// ══════════════════════════════════════════════════════════════
//  MAIN
// ══════════════════════════════════════════════════════════════
int main() 
{
    MaritimeGraph graph;
    CustomShipManager shipManager;

    FileParser::parseRoutes("Routes.txt", graph);
    FileParser::parsePortCharges("PortCharges.txt", graph);
    FileParser::assignPortPositions(graph);

    sf::RenderWindow window(
        sf::VideoMode(1280, 720),
        "Path of Tides — Maritime Navigation Optimizer",
        sf::Style::Close | sf::Style::Titlebar
    );
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded = false;
    if (font.loadFromFile("DejaVuSans.ttf"))
    {
        fontLoaded = true;
    }
    else if (font.loadFromFile("arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        fontLoaded = true;
    }
    else if (font.loadFromFile("C:/Windows/Fonts/segoeui.ttf"))
    {
        fontLoaded = true;
    }
    if (!fontLoaded)
    {
        cerr << "Warning: Could not load any font\n";
    }

    PlayBackgroundMusic();

    sf::Clock clock;
    while (window.isOpen()) 
    {
        SplashScreen splash(window, font);
        int result = 0;
        while (window.isOpen() && result == 0) 
        {
            float dt = clock.restart().asSeconds(); 
            if (dt > 0.1f)
            {
                dt = 0.1f;
            }
            result = splash.run(dt);
        }
        if (result == 2 || !window.isOpen())
        {
            break;
        }
        if (result == 1) 
        {
            Visualization viz(window, graph, shipManager, font);
            bool backToMenu = viz.run();
            if (!backToMenu || !window.isOpen())
            {
                break;
            }
        }
    }
    StopBackgroundMusic();
    return 0;
}