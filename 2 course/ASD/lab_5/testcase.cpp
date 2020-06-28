#include "tree.h"
#include "queue.h"
#include "doctest.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

using std::cout;
using std::endl;
using namespace std::chrono;
using sec = duration<double, std::ratio<1>>;
using lodger = Queue::Container::value_t;


std::string readFromFile(const std::string &flName)
{
    std::string A;
    std::ifstream B(flName);
    std::getline(B,A,'\0');
    B.close();
    return A;
}

// -- ��������������� ����� -
TEST_CASE("����� Tree")
{
    SUBCASE("���� �������� ������ � ���������� ��������� � ������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        a.insert(5,lodger);
        a.insert(8,lodger);
        a.insert(13,lodger);
        a.insert(16,lodger);
        a.insert(11,lodger);
        CHECK(a.count() == 8);
        CHECK(a.insert(10,lodger) == false); // ���� ���������
    }
    SUBCASE("���� ������������ �����������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        a.insert(5,lodger);
        a.insert(8,lodger);
        a.insert(13,lodger);
        a.insert(16,lodger);
        a.insert(11,lodger);
        CHECK(a.count() == 8);
        Tree b(a);
        CHECK(b.count() == 8);
        std::string fileName = "testcopy1.txt";
        std::string fileName2 = "testcopy2.txt";
        a.PreOrderRecurse(fileName);
        b.PreOrderRecurse(fileName2);
        CHECK(readFromFile(fileName) == readFromFile(fileName2));
    }
    SUBCASE("���� ������������ �� data � ������������ �����������")
    {
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        Tree::value_t lodger2{3,2011,4,65.3f,3,89.7f};
        Tree a({1,lodger});
        CHECK(elemToString(a.get_root_data()) == "1 3 2014 4 60.3 3 87.7");
        CHECK(a.count() == 1);
        Tree b(Tree({2,lodger2}));
        CHECK(b.count() == 1);
        CHECK(elemToString(b.get_root_data()) == "2 3 2011 4 65.3 3 89.7");
        a = Tree({37,lodger2});
        CHECK(a.count() == 1);
        CHECK(elemToString(a.get_root_data()) == "37 3 2011 4 65.3 3 89.7");
    }
    SUBCASE("���� �������")
    {
        Tree::value_t lodger_1(2,2014,2,22.1f,5,71.3f);
        Tree::value_t lodger_2(2,2011,5,48.0f,2,55.9f);
        Tree::value_t lodger_3(5,2017,3,54.3f,4,60.5f);
        Tree::value_t lodger_4(3,2019,4,60.3f,3,87.7f);
        Tree::value_t lodger_5(1,2010,1,44.8f,3,88.2f);
        Tree::value_t lodger_6(1,2010,2,41.8f,3,69.2f);
        Tree::value_t lodger_7(1,2010,2,41.8f,3,69.2f);

        Tree a {{9,lodger_1},{7,lodger_2},{10,lodger_3},{5,lodger_4},
               {8,lodger_5},{13,lodger_6},{16,lodger_7}};

        std::string pre1 = "pre1.txt";
        std::string in1 = "in1.txt";
        std::string post1 = "post1.txt";
        std::string pre2 = "pre2.txt";
        std::string in2 = "in2.txt";
        std::string post2 = "post2.txt";
        // 9 7 5 8 10 13 11 16
        a.PreOrderRecurse(pre1);
        a.PreOrderIterative(pre2);
        CHECK(readFromFile(pre1) == readFromFile(pre2));
        // 5 7 8 9 10 11 13 16
        a.InOrderRecurse(in1);
        a.InOrderIterative(in2);
        CHECK(readFromFile(in1) == readFromFile(in2));
        // 5 8 7 11 16 13 10 9
        a.PostOrderRecurse(post1);
        a.PostOrderIterative(post2);
        CHECK(readFromFile(post1) == readFromFile(post2));
    }
    SUBCASE("���� ������ � ������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        a.insert(5,lodger);
        a.insert(8,lodger);
        a.insert(13,lodger);
        a.insert(16,lodger);
        a.insert(11,lodger);
        auto res = a.find(13);
        CHECK(a.get_key(res) == 13);
        res = a.find(15);   // ��� ��� � �������� ������
        CHECK(a.get_key(res) == a.get_root_key());
    }
    SUBCASE("���� ����������� ���� ��������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        CHECK(a.count() == 3);
        Tree b;
        b.insert(5,lodger);
        b.insert(8,lodger);
        b.insert(13,lodger);
        b.insert(16,lodger);
        b.insert(11,lodger);
        CHECK(b.count() == 5);
        Tree c;

        a.concat(b);
        CHECK(a.count() == 8);
        CHECK(a.get_key(a.find(9)) == 9);
        CHECK(a.get_key(a.find(7)) == 7);
        CHECK(a.get_key(a.find(10)) == 10);
        CHECK(a.get_key(a.find(5)) == 5);
        CHECK(a.get_key(a.find(8)) == 8);
        CHECK(a.get_key(a.find(13)) == 13);
        CHECK(a.get_key(a.find(16)) == 16);
        CHECK(a.get_key(a.find(11)) == 11);
    }
    SUBCASE("���� ������ �������� � ���������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        a.insert(5,lodger);
        a.insert(8,lodger);
        a.insert(13,lodger);
        a.insert(16,lodger);
        a.insert(11,lodger);
        auto max = a.find_max();
        auto min = a.find_min();
        CHECK(a.get_key(max) == 16);
        CHECK(a.get_key(min) == 5);
    }
    SUBCASE("���� ��������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        a.insert(5,lodger);
        a.insert(8,lodger);
        a.insert(13,lodger);
        a.insert(16,lodger);
        a.insert(11,lodger);
        a.insert(6, lodger);
        CHECK(a.count() == 9);
        CHECK(!a.empty());
        std::string pre1 = "delete.txt";
        a.erase(8); // ������� ����
        a.erase(5); // ������� ���� � ����� �������� ������
        a.erase(13);   // ������ ���� � ����� ������������
        a.erase(7);   // ������ ���� � �������� �����
        a.PreOrderRecurse(pre1);
        std::string rightResult = "9 3 2014 4 60.3 3 87.7\n"
                                  "6 3 2014 4 60.3 3 87.7\n"
                                  "10 3 2014 4 60.3 3 87.7\n"
                                  "11 3 2014 4 60.3 3 87.7\n"
                                  "16 3 2014 4 60.3 3 87.7\n";
        CHECK(readFromFile(pre1) == rightResult);
        a.clear();
        CHECK(a.count() == 0);
        CHECK(a.empty());
    }
    SUBCASE("���� ������ ������")
    {
        Tree a;
        Tree::value_t lodger{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger);
        a.insert(7,lodger);
        a.insert(10,lodger);
        a.insert(5,lodger);
        a.insert(8,lodger);
        a.insert(13,lodger);
        a.insert(16,lodger);
        a.insert(11,lodger);
        CHECK(a.height() == 3);

        Tree b;
        b.insert(5,lodger);
        b.insert(4,lodger);
        b.insert(3,lodger);
        b.insert(2,lodger);
        b.insert(1,lodger);
        CHECK(b.height() == 4);
    }
    SUBCASE("���� ������")
    {
        Tree a;
        Tree::value_t lodger1{1,2017,2,62.3f,4,93.7f};
        Tree::value_t lodger2{3,2014,4,60.3f,3,87.7f};
        a.insert(9,lodger1);
        a.insert(7,lodger1);
        a.insert(10,lodger1);
        a.insert(5,lodger1);
        a.insert(8,lodger1);
        a.insert(13,lodger1);
        a.insert(16,lodger1);
        a.insert(11,lodger1);
        CHECK(elemToString(a.find(8)) == elemToString({8,lodger1}));
        a.replace(8,lodger2);
        CHECK(elemToString(a.find(8)) == elemToString({8,lodger2}));
    }
}

TEST_CASE("����� Queue")
{
    Queue t;         // -- ������� ������ � ����� ���� --
    SUBCASE("���� �������� � ������������") // -- �������� � ������������ --
    {
        Queue a("Ivan");
        CHECK(a.get_Name() == "Ivan");
        Queue b("Vanya", "17.11.18");
        CHECK(b.get_Name() == "Vanya");
        CHECK(b.get_Date() == "17.11.18");
        lodger a_lodger(3,2017,4,62.3f,3,83.7f);
        lodger b_lodger(3,2014,4,60.3f,3,87.7f);
        lodger c_lodger(3,2011,4,57.3f,3,89.7f);
        b.add(4,a_lodger);
        b.add(8,b_lodger);
        b.add(1,c_lodger);
        CHECK(b.get_Count_of_people() == 3);
        CHECK(elemToString({8,b.find(8)}) == "8 3 2014 4 60.3 3 87.7");
        b.replace(8,c_lodger);
        CHECK(elemToString({8,b.find(8)}) == "8 3 2011 4 57.3 3 89.7");
    }
}
