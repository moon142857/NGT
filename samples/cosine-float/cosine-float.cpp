
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <mutex>
#include <memory>
#include <stdexcept>
#include "NGT/Index.h"

using namespace std;

float A1[192] = {0.027904, 0.008165, -0.016216, 0.038300, -0.064714, 0.091668, -0.125231, 0.016729, 0.041737, 0.073088, 0.050529, 0.074768, 0.092020, 0.021182, 0.037864, -0.036610, -0.014878, 0.004795, 0.023840, -0.014669, -0.053501, 0.025635, 0.007833, -0.019397, -0.004728, -0.041092, 0.016046, 0.067942, -0.012732, 0.111436, 0.105730, -0.036629, 0.013017, 0.041272, -0.016910, -0.010036, -0.001377, -0.037550, -0.025227, -0.116117, 0.010197, -0.082421, 0.116458, -0.004367, -0.025777, -0.069945, -0.152072, -0.061343, -0.095305, 0.055599, 0.022037, -0.001462, -0.145644, 0.180299, -0.093235, 0.014251, 0.083256, 0.005355, -0.054308, 0.006741, 0.008279, 0.062321, -0.009001, 0.006057, -0.053757, -0.082459, -0.073677, -0.070126, 0.025379, -0.005184, 0.023727, -0.036022, 0.065692, -0.051469, 0.090263, -0.106024, 0.004320, -0.056368, -0.037607, -0.030002, 0.053710, -0.083570, 0.064533, -0.097697, 0.018505, 0.006371, 0.080323, -0.019017, -0.134821, 0.143204, -0.014868, -0.127064, -0.040816, -0.003247, -0.087301, -0.050425, -0.077484, 0.016425, -0.033297, 0.102350, -0.078595, 0.092428, -0.050168, -0.017232, -0.247538, 0.079639, -0.007425, 0.038319, 0.049409, 0.004795, 0.089314, 0.013662, -0.029746, -0.021590, -0.115404, -0.015713, 0.079117, -0.005991, 0.052646, -0.205962, 0.064533, -0.002791, 0.165725, 0.041073, -0.021600, -0.055409, -0.093226, 0.018932, -0.037949, 0.017422, -0.039839, -0.008982, -0.038975, -0.004348, -0.131478, 0.014640, -0.057868, 0.001709, 0.006523, -0.036202, -0.088488, 0.119791, 0.053197, 0.136957, -0.051213, -0.018780, -0.073847, 0.109395, -0.137935, 0.081035, -0.044918, 0.089266, 0.021999, -0.199145, 0.078129, -0.020100, 0.094431, 0.043057, -0.095675, 0.041234, -0.161899, 0.161965, -0.016254, 0.104752, -0.008773, 0.088421, -0.006190, -0.013634, -0.007709, 0.046874, -0.108939, -0.029214, -0.034360, -0.043864, 0.046722, -0.047368, 0.092580, -0.008260, -0.068806, -0.040987, -0.042174, 0.046285, -0.051830, 0.157256, 0.107306, 0.001082, 0.031873, -0.016900, 0.013957, -0.124680, 0.004861, -0.035243};
float A2[192] = {-0.102066, 0.169254, 0.020833, -0.061236, -0.110985, 0.078217, -0.129061, -0.107352, 0.006430, 0.068502, 0.102634, 0.013288, 0.107043, -0.058021, 0.000538, -0.050117, 0.031255, -0.029364, 0.072195, 0.017310, 0.026378, -0.049431, 0.024546, -0.020196, -0.035794, -0.047828, 0.106655, 0.036053, -0.010312, 0.109213, 0.120003, 0.059563, 0.034321, 0.009864, 0.050635, -0.101320, -0.011517, -0.021819, 0.047290, -0.001314, 0.061952, -0.046882, 0.076186, 0.090122, -0.060867, -0.153617, 0.026059, -0.066352, -0.021460, -0.003653, -0.004320, 0.083254, -0.020973, 0.265368, -0.162177, 0.028826, -0.003534, 0.045648, -0.100394, -0.046723, 0.074116, -0.101608, -0.012820, -0.083015, 0.002588, -0.049421, 0.048863, -0.099558, 0.016444, 0.048923, -0.066820, 0.005047, -0.030707, -0.050914, 0.050944, -0.025601, 0.074892, 0.030110, -0.078874, 0.066282, -0.045578, -0.086817, -0.036033, -0.015030, -0.049371, -0.048525, 0.068542, -0.029861, -0.109283, 0.114260, 0.012602, -0.099558, 0.088828, 0.014642, -0.071538, -0.051919, -0.009914, -0.087175, -0.007067, 0.075121, -0.098771, 0.022864, 0.028796, 0.017160, -0.075081, 0.063017, 0.027910, -0.093775, -0.015538, 0.026885, 0.084398, 0.040821, 0.035445, -0.070961, -0.109641, -0.074395, 0.100195, -0.005405, 0.138189, -0.140050, 0.010860, -0.004041, 0.116290, 0.001394, 0.014911, -0.074106, -0.004678, 0.018932, -0.083562, 0.019161, 0.010352, -0.029294, -0.120829, 0.065814, -0.080576, 0.009357, -0.095148, 0.055413, 0.054806, 0.000707, -0.032360, 0.036292, 0.057015, 0.099697, -0.035764, -0.051073, -0.091864, -0.016205, -0.068850, 0.128603, 0.010103, -0.001891, -0.020594, -0.130614, -0.021062, -0.076784, 0.046634, -0.050973, -0.053462, 0.067507, -0.179547, -0.026258, 0.004459, 0.033853, 0.000269, 0.145783, -0.019718, -0.050804, 0.017399, 0.068960, -0.092650, 0.001732, 0.009924, -0.066004, 0.073200, -0.078097, 0.094233, -0.041955, -0.013677, -0.063664, -0.027164, 0.083881, 0.098951, 0.111144, 0.011686, 0.197881, -0.013119, -0.035007, 0.077659, -0.164954, 0.008560, 0.045578};

float B1[192] = {0.149197, 0.008954, -0.117991, -0.007878, -0.015739, 0.060657, 0.014585, -0.114740, 0.058269, 0.098198, 0.006319, -0.070624, -0.019881, 0.031760, -0.051818, 0.047782, 0.052426, -0.010998, 0.061529, 0.082591, -0.091228, 0.021441, 0.080697, 0.087341, -0.103327, 0.098965, -0.135185, 0.066958, -0.071126, -0.014056, -0.053395, -0.105134, -0.011888, -0.021573, -0.032377, 0.044098, -0.004221, -0.057934, 0.099679, -0.032113, -0.014585, -0.089668, -0.066755, -0.005649, -0.000599, -0.098745, 0.126725, -0.064693, -0.125429, -0.107795, -0.137687, -0.030747, -0.011694, 0.074360, 0.032642, 0.065803, -0.071558, 0.109355, 0.093475, -0.013659, -0.035479, 0.025274, 0.046583, 0.126302, 0.019784, -0.047200, 0.106808, -0.074730, -0.002600, 0.027777, -0.121552, -0.039604, -0.012628, 0.056550, 0.131060, -0.019511, 0.043472, 0.043895, -0.009377, 0.066597, -0.090575, -0.010892, -0.156458, 0.131827, -0.137767, 0.068861, 0.028949, -0.047209, -0.080600, 0.057484, -0.037030, 0.065142, 0.108747, 0.010108, 0.111285, 0.047209, -0.028517, -0.065918, -0.037489, 0.074475, 0.039075, -0.009236, 0.016982, 0.050302, 0.001366, 0.028130, -0.023662, 0.005208, 0.015448, 0.108712, -0.007693, -0.029998, -0.064693, -0.021652, 0.063829, 0.066482, -0.077251, 0.051677, 0.006301, -0.049861, -0.024367, -0.056286, -0.084680, -0.046627, 0.076616, 0.164196, 0.196132, 0.082080, -0.031919, 0.012355, -0.040176, 0.185064, 0.055704, 0.069751, 0.043516, 0.006319, 0.050760, 0.049967, -0.059115, 0.045878, 0.123032, -0.027724, -0.046654, -0.021811, -0.053254, -0.100049, -0.020163, 0.023477, -0.091862, 0.024014, -0.054021, -0.004803, -0.107593, -0.047782, 0.127509, -0.071285, 0.036678, -0.156537, 0.063248, -0.007156, -0.016338, -0.102869, -0.108668, 0.094374, -0.019661, -0.002194, 0.032386, 0.014946, 0.019449, 0.002397, -0.007253, -0.061115, -0.094964, -0.064596, -0.046662, 0.051421, -0.050276, 0.049897, 0.001710, -0.057854, 0.080053, 0.069901, -0.140675, -0.016964, 0.041578, 0.139617, -0.048425, -0.059405, 0.000291, -0.040124, 0.182905, -0.049853};
float B2[192] = {0.127552, -0.007579, -0.030017, -0.015554, -0.066014, 0.093426, -0.058764, -0.040634, 0.071318, 0.038123, -0.059225, -0.030731, 0.103037, -0.092335, -0.013617, 0.084229, -0.019362, 0.028973, -0.034991, 0.117556, -0.048185, 0.082838, 0.130261, 0.010053, -0.132903, 0.108435, -0.131229, 0.137934, 0.028315, 0.118628, -0.049962, -0.041894, -0.127881, 0.025936, -0.065017, 0.123208, -0.027261, -0.060936, 0.116945, 0.024365, 0.007316, 0.025409, -0.036919, -0.022691, 0.003667, -0.023876, 0.060880, 0.045608, 0.014247, 0.044122, -0.106864, -0.011924, -0.071120, -0.007241, 0.049840, 0.039486, -0.015977, 0.044884, 0.023218, 0.107504, 0.011520, 0.086138, 0.096116, 0.130035, -0.011049, -0.001025, 0.158453, -0.119221, -0.079603, 0.072023, -0.130627, -0.023754, 0.148494, 0.048768, -0.070613, -0.066503, 0.095429, -0.065817, -0.032433, 0.019278, -0.111886, -0.027675, -0.055294, 0.038113, -0.079471, 0.157362, 0.094009, 0.050940, -0.081455, 0.160541, -0.039703, 0.096717, 0.074026, 0.027261, 0.057147, 0.003047, -0.070462, -0.072540, -0.097442, 0.067622, 0.012855, 0.033233, 0.034295, 0.052435, -0.005332, -0.041508, -0.017698, 0.012084, -0.053488, 0.088715, -0.093191, 0.006037, -0.090248, -0.032866, 0.054213, 0.045373, 0.017246, 0.039825, 0.010297, -0.015196, -0.082405, -0.091677, -0.070086, -0.010918, 0.169888, 0.085113, 0.122578, 0.063014, 0.019701, -0.045853, -0.130214, 0.031108, 0.082781, 0.074177, -0.027948, 0.115421, 0.035650, 0.097630, -0.043784, 0.043464, 0.068130, -0.053347, -0.031098, -0.075719, -0.038104, -0.123283, -0.021694, 0.127402, -0.085226, -0.054730, -0.075014, 0.010607, -0.067801, 0.069672, -0.025983, -0.092269, -0.029039, -0.136768, 0.085104, 0.065328, -0.049539, 0.015996, -0.037295, 0.045862, 0.026133, 0.069879, -0.043267, 0.040060, -0.003037, -0.047216, -0.037483, -0.011943, -0.106074, -0.067434, -0.067538, 0.068976, -0.035048, -0.019898, -0.053573, 0.068939, 0.043408, 0.074449, -0.067660, 0.041292, -0.041489, 0.194046, 0.052520, 0.014218, 0.012874, -0.031117, 0.076499, 0.008416};

size_t getmemused()
{
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);
    return (size_t)(rusage.ru_maxrss);
}

size_t getms()
{
    struct timeval time_v = {0};
    gettimeofday(&time_v, NULL);
    return (size_t)time_v.tv_sec * 1000 + time_v.tv_usec / 1000;
}

#define ALPHA (2.5544236818f)
#define BETA (-3.9668512074f)
#define DIM (192)
#define THRESHOLD (0.789999f)

int main(int argc, char **argv)
{
    string indexPath = "index1";
    //string	objectFile	= "./data/sift-dataset-5k.tsv";
    //string	queryFile	= "./data/sift-query-3.tsv";
    // index construction
    size_t ret = 0;

    float mul = 0.0f;
    float cosin = 0.0f;
    for (size_t i = 0; i < 192; i++)
    {
        cosin += A1[i] * A2[i];
        mul+= (A1[i] - A2[i])*(A1[i] - A2[i]);
    }
    printf("cosin %f mul %f %f %f\n", cosin, mul, 1.0-mul/(float)4, 
        1 / (float)(1 + std::exp(ALPHA*(mul) + BETA)));

    try
    {
        NGT::Property property;
        property.dimension = 192;
        property.objectType = NGT::ObjectSpace::ObjectType::Float;
        property.distanceType = NGT::Index::Property::DistanceType::DistanceTypeL2;
        NGT::Index::create(indexPath, property);
        NGT::Index index(indexPath);
        index.enableLog();
        

        NGT::Property prop1;
        index.getProperty(prop1);
        cout<<prop1;

        vector<float> AA1(A1, A1 + 192);
        vector<float> AA2(A2, A2 + 192);
        vector<float> BB1(B1, B1 + 192);
        vector<float> BB2(B2, B2 + 192);


        ret = index.append(AA1);
        cout << "append " << ret << endl;
        ret = index.append(AA2);
        cout << "append " << ret << endl;
        ret = index.append(BB1);
        cout << "append " << ret << endl;
        ret = index.append(BB2);
        cout << "append " << ret << endl;

        for(size_t j = 0; j< 192;++j)
            {
                AA1[j] = 0;
                AA2[j] = 0;
                BB1[j] = 0;
                BB2[j] = 0;
            }


        size_t m1, t1;

        m1 = getmemused(); t1 = getms();
        index.createIndex(16);
        cout << "demo createIndex mem: " << getmemused() - m1 << " time: " << getms() - t1 <<  endl;

        m1 = getmemused(); t1 = getms();
        for (size_t i = 0; i < 100; i++)
        {
            float pp[192];
            
            float mul = 0.0f;
            for(size_t j = 0; j< 192;++j)
            {
                pp[j] = (rand() % 10) * 0.1f;
                mul += pp[j] * pp[j];
            }
            mul = std::sqrt(mul);
            for(size_t j = 0; j< 192;++j)
            {
                pp[j] = pp[j] / mul;
                //cout << " " << pp[j];
            }
            //cout <<  endl;
            vector<float> tmp(pp, pp + 192);
            ret = index.append(tmp);
            
            //cout << "append " << ret << " pp " << pp <<  endl;
            //tmp[0] = 1.f;
            //tmp[1] = 2.f;

            //NGT::ObjectSpace &objectSpace = index.getObjectSpace();
            //float *object = static_cast<float *>(objectSpace.getObject(ret));
            //cout << "get  " << ret << " pp " << object << " " << object[0] <<  " " << object[1] << endl;
            //delete[] object;

            //index.remove(ret,true);
        }
        
        cout << "demo append mem: " << getmemused() - m1 << " time: " << getms() - t1 <<  endl;
        

       

        std::filebuf fb;
        fb.open ("test.txt",std::ios::out);
        std::ostream os(&fb);
        index.version(os);
        
        // for (size_t i = 0; i < 192; i++)
        // {
        //     A1[i] = 0.000001;
        //     A2[i] = 0.000001;
        //     B1[i] = 0.000001;
        // }

        cout << "getPath: " << index.getPath() << endl;
        //index.remove(4,true);
#if 0
    ifstream	is(objectFile);
    string	line;
    while (getline(is, line)) {
      vector<float>	obj;
      stringstream	linestream(line);
      while (!linestream.eof()) {
	float value;
	linestream >> value;
	if (linestream.fail()) {
	  obj.clear();
	  break;
	}
	obj.push_back(value);
      }
      if (obj.empty()) {
	cerr << "An empty line or invalid value: " << line << endl;
	continue;
      }
      obj.resize(property.dimension);  // cut off additional data in the file.
      index.append(obj);
    }
#endif

        float a = 0;//index.getEpsilonFromExpectedAccuracy(0.1);
        size_t b = index.getObjectRepositorySize();
        size_t c = index.getSizeOfElement();
        size_t d = index.getSharedMemorySize(os);
        fb.close();

        //prop1.save("CONF");

        printf("getEpsilonFromExpectedAccuracy %f\ngetObjectRepositorySize %zu\ngetSizeOfElement %zu\ngetSharedMemorySize %zu\n", a, b, c, d);
        
        //index.save();
        //index.saveIndex("index2");

        vector<float> query(A2, A2 + 192);

        //ifstream		is(queryFile);
        //string		line;
        //while (getline(is, line)) {
        // vector<float>	query;
        // {
        //   stringstream	linestream(line);
        //   while (!linestream.eof()) {
        //     float value;
        //     linestream >> value;
        //     query.push_back(value);
        //   }
        //   query.resize(property.dimension);
        //   cout << "Query : ";
        //   for (size_t i = 0; i < 5; i++) {
        //     cout << query[i] << " ";
        //   }
        //   cout << "...";
        // }
        NGT::SearchQuery sc(query);
        NGT::ObjectDistances objects;
        sc.setResults(&objects);
        //sc.setSize(100);
        //sc.setEpsilon(1.1);
        float vv = 0.867649f;
        float ss = std::sqrt((std::log(1.0f / vv - 1.0f) - BETA) / ALPHA);
        cout << "ss "<< ss << "vv " << vv<< endl;
        sc.setRadius(ss);

        m1 = getmemused(); t1 = getms();
        index.linearSearch(sc);
        //index.search(sc);
        cout << "demo search mem: " << getmemused() - m1 << " time: " << getms() - t1 <<  endl;
        cout << endl
             << "Rank\tID\tDistance\tss" << std::showbase << endl;
        for (size_t i = 0; i < objects.size(); i++)
        {
            //cout << i + 1 << "\t" << objects[i].id << "\t" << objects[i].distance * objects[i].distance << "\t: ";
            cout << i + 1 << "\t" << objects[i].id << "\t" << objects[i].distance  << "\t" <<1 / (float)(1 + std::exp(ALPHA*( objects[i].distance * objects[i].distance ) + BETA))<< "\t: ";
            NGT::ObjectSpace &objectSpace = index.getObjectSpace();
            float *object = static_cast<float *>(objectSpace.getObject(objects[i].id));
            for (size_t idx = 0; idx < 5; idx++)
            {
                cout << object[idx] << " ";
            }
            cout << endl;
        }
        cout << endl;

 //objects[i].distance  << "\t" <<1 / (float)(1 + std::exp(ALPHA*( objects[i].distance * objects[i].distance ) + BETA))
        usleep(1000000000);
    }
    catch (NGT::Exception &err)
    {
        cerr << "Error " << err.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "Error" << endl;
        return 1;
    }
    return 0;

    // nearest neighbor search
    try
    {
        NGT::Index index(indexPath);
        NGT::Property property;
        index.getProperty(property);

        vector<float> query(A2, A2 + 192);

        //ifstream		is(queryFile);
        //string		line;
        //while (getline(is, line)) {
        // vector<float>	query;
        // {
        //   stringstream	linestream(line);
        //   while (!linestream.eof()) {
        //     float value;
        //     linestream >> value;
        //     query.push_back(value);
        //   }
        //   query.resize(property.dimension);
        //   cout << "Query : ";
        //   for (size_t i = 0; i < 5; i++) {
        //     cout << query[i] << " ";
        //   }
        //   cout << "...";
        // }
        NGT::SearchQuery sc(query);
        NGT::ObjectDistances objects;
        sc.setResults(&objects);
        sc.setSize(2);
        //sc.setEpsilon(1.1);

        index.search(sc);
        cout << endl
             << "Rank\tID\tDistance" << std::showbase << endl;
        for (size_t i = 0; i < objects.size(); i++)
        {
            //cout << i + 1 << "\t" << objects[i].id << "\t" << objects[i].distance * objects[i].distance << "\t: ";
            cout << i + 1 << "\t" << objects[i].id << "\t" <<1 / (float)(1 + std::exp(ALPHA*( objects[i].distance * objects[i].distance ) + BETA))<< "\t: ";
            NGT::ObjectSpace &objectSpace = index.getObjectSpace();
            float *object = static_cast<float *>(objectSpace.getObject(objects[i].id));
            for (size_t idx = 0; idx < 5; idx++)
            {
                cout << object[idx] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    catch (NGT::Exception &err)
    {
        cerr << "Error " << err.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "Error" << endl;
        return 1;
    }

    return 0;
}
