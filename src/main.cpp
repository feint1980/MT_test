// #include <iostream>

// //#include "FileObject.h"

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #define ago = 0

// void displayMatrix(const glm::mat4 & matrix)
// {
//     std::cout << "matrix: \n";
//     for (int i = 0; i < 4; i++)
//     {
//         std::cout << matrix[i][0] << "\t\t" << matrix[i][1] << "\t\t" << matrix[i][2] << "\t\t" << matrix[i][3] << "\n";
//     }
// }

// int main(int argc, char const *argv[])
// {
    
//     long long time ago;

//     std::cout << "test \n";

//     glm::vec3 translate(1.0f, 7.0f, 3.0f);

//     glm::mat4 orthoMatrix  = glm::mat4(1.0f);

//     std::cout << "before translate : \n";

//     displayMatrix(orthoMatrix);

//     std::cout << "after translate : \n";

//     orthoMatrix = glm::translate(orthoMatrix, translate);

//     displayMatrix(orthoMatrix);

//     glm::vec3 scale(1.0f, 3.0f, 2.0f);

//     std::cout << "after scale : \n";

//     orthoMatrix = glm::scale(glm::mat4(1.0f), scale) * orthoMatrix;

//     displayMatrix(orthoMatrix);

//     glm::vec3 rotation(0.0f, 1.0f, 0.0f);

//     std::cout << "after rotation : \n";

//     orthoMatrix = glm::rotate(orthoMatrix, glm::radians(360.0f), rotation);

//     displayMatrix(orthoMatrix);

//     std::cout << "end \n";

//     return 0;
// }



#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <mutex>
#include <dirent.h>
typedef std::vector<std::string> buffer;




void readFile(const std::string & filePath, std::vector<buffer>& buffs)
{
    std::cout << "called tReadFile \n";
    std::mutex m_values_mutex;
    std::ifstream file(filePath);
    buffer buff;
    //m_values_mutex.lock();
    buff.push_back(filePath);
    //m_values_mutex.unlock();
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {   
        //	m_values_mutex.lock();
            buff.push_back(line);
        //	m_values_mutex.unlock();
            //std::cout << line << '\n';
        }
        file.close();
    }
    m_values_mutex.lock();
    buffs.push_back(buff);
    m_values_mutex.unlock();

}


int main()
{
    auto start = std::chrono::system_clock::now();
    std::vector<buffer> mainBuffers;
    //
     

    std::vector<std::thread> threads;
 
    // std::thread t;
    // std::thread t2;
    // std::thread t3;

    // t = std::thread(readFile, "../data/evil_sealing_circle.xml", std::ref(mainBuffers));
    // t2 = std::thread(readFile, "../data/lightning_effect.xml", std::ref(mainBuffers));
    // t3 = std::thread(readFile, "../data/shiki.xml", std::ref(mainBuffers));
    // t.join();
    // t2.join();
    // t3.join();



    // readFile("../data/evil_sealing_circle.xml", std::ref(mainBuffers));
    // readFile("../data/lightning_effect.xml", std::ref(mainBuffers));
    // readFile("../data/shiki.xml", std::ref(mainBuffers));


    std::string filePath = "../data/";
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(filePath.c_str())) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            std::string fileName = entry->d_name;
            if (fileName != "." && fileName != "..")
            {
                std::string fullPath = filePath + fileName;
                std::thread t(readFile, fullPath, std::ref(mainBuffers));
                threads.push_back(std::move(t));
               // readFile(fullPath, std::ref(mainBuffers));
            }
        }
        closedir(dir);
    }
    else
    {
        std::cout << "error \n";
    }


    for (int i = 0; i < threads.size(); i++)
    {
        
        threads[i].join();
    }
    

    // readFile("../data/evil_sealing_circle.xml");
    // readFile("../data/lightning_effect.xml");
    // readFile("../data/shiki.xml");


    // write files
    std::cout << "total size : " << mainBuffers.size() << "\n";
    std::ofstream file("../data/result.xml");
    if (file.is_open())
    {
        for (int i = 0; i < mainBuffers.size(); i++)
        {

            for (int j = 1; j < mainBuffers[i].size(); j++)
            {
                file << mainBuffers[i][j] << "\n";
            }

        }
        file.close();
    }
        std::cout << " write done \n";


    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;
    std::cout << elapsed.count() << '\n';
    // if(t.joinable())
    // {
    //     t.join();
    // }

    // if(t2.joinable())
    // {
    //     t2.join();
    // }





    
   
 

}

