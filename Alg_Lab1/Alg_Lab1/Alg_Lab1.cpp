// Alg_Lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>

const float epsilon = 0.000001f;

const int acrePriceLowerBound = 17;
const int acrePriceUpperBound = 26;

const int collectedWheatLowerBound = 1;
const int collectedWheatUpperBound = 6;

const float ratAteLowerBound = 0;
const float ratAteUpperBound = 0.07f;

const float diedCitizensForLose = 0.45f;

const float plagueProbability = 0.15f;


const int citizenWheatConsumption = 20;
const int citizenCultivationAcres = 10;
const float wheatForAcre = 0.5f;
const int rounds = 10;

float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

bool is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

bool is_file_exist(const char* fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

void clear_file()
{
    std::ofstream ofs;
    ofs.open("save.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

bool load_data(int& year, int& diedHungry, int& newCitizens, bool& isPlague, int& citizensAmount, int& wheatCollected, int& wheatFromAcre, int& ratsAte, float& totalWheat, int& acres, int& acrePrice, int& P, float& L)
{
    std::ifstream file;
    file.open("save.txt");

    if (!is_empty(file))
    {
        std::cout << "Мы обнаружили сохранение вашей прошлой игры. Хотите ли вы продолжить ее?" << std::endl;
        std::cout << "1. Да" << std::endl;
        std::cout << "2. Нет" << std::endl;

        char variant = 0;
        std::cin >> variant;
        if (variant == '1')
        {
            file >> year >> diedHungry >> newCitizens >> isPlague >> citizensAmount >> wheatCollected >> wheatFromAcre >> ratsAte >> totalWheat >> acres >> acrePrice>>P>>L;
            return true;
        }
        else
        {
            file.close();
            clear_file();
        }
    }
    return false;
}

void save_data(int& year, int& diedHungry, int& newCitizens, bool& isPlague, int& citizensAmount, int& wheatCollected, int& wheatFromAcre, int& ratsAte, float& totalWheat, int& acres, int& acrePrice, int& P, float& L)
{
    std::ifstream file;
    file.open("save.txt");
    if (!is_empty(file))
    {
        file.close();
        clear_file();
    }

    std::ofstream writeFile;
    writeFile.open("save.txt");

    writeFile << year << std::endl<< diedHungry << std::endl << newCitizens << std::endl << isPlague << std::endl << citizensAmount << std::endl << wheatCollected <<
        std::endl << wheatFromAcre << std::endl << ratsAte << std::endl << totalWheat << std::endl << acres << std::endl << acrePrice<< std::endl << P<< std::endl<< L;
    writeFile.close();
}

int count_new_citizens(int died,int wheatFromAcre,int wheatCollected)
{
    int newCitizens= (int) died / 2 + (5 - wheatFromAcre) * wheatCollected / 600 + 1;

    return clamp(newCitizens, 0, 50);
}

bool check_inputed_values(int newAcres, int soldAcres,int wheatToEat, int acresToSeed, int wheat, int citizens, int acres, int acrePrice)
{

    if (wheatToEat > wheat + soldAcres*acrePrice)
    {
        std::cout << "О, повелитель! У нас нет столько пшеницы для голодающих..." << std::endl;
        return false;
    }

    if (acrePrice * newAcres > wheat)
    {
        std::cout << "О, повелитель! У нас нет столько пшеницы для покупки земли..." << std::endl;
        return false;
    }

    if (acresToSeed > citizens * citizenCultivationAcres)
    {
        std::cout << "О, повелитель! У нас мало горожан для обработки стольких акров... " << std::endl;
        return false;
    }


    if (soldAcres > acres)
    {
        std::cout << "О, повелитель! У нас нет столько акров для продажи..." << std::endl;
        return false;
    }

    if (acresToSeed > acres + newAcres - soldAcres)
    {
        std::cout << "О, повелитель! Мы не сможем засеять, у нас нет так много земли..." << std::endl;
        return false;
    }

    if (wheat + soldAcres*acrePrice - wheatToEat - newAcres * acrePrice - acresToSeed * wheatForAcre<0)
    {
        std::cout << "О, повелитель! Где же нам взять столько пшеницы..." << std::endl;
        return false;
    }

    if (acres + newAcres - soldAcres < 0)
    {
        std::cout << "О, повелитель! Откуда нам взять столько земли..." << std::endl;
        return false;
    }

    if (newAcres >= 0 && soldAcres >= 0 && wheatToEat >= 0 && acresToSeed >= 0)
        return true;
    else
    {
        std::cout << "Я не понимаю, ваше величество..." << std::endl;
        return false;
    }

}

void print_game(int roundNum, int died, int newCitizens, bool wasPlague, int citizens, int collectedWheat, int wheatFromAcre, int ratAte,int totalWheat, int acres, int acrePrice)
{
    std::cout << "Мой повелитель, соизволь поведать тебе" << std::endl;
    std::cout << "\tв году " << roundNum << " твоего высочайшего правления" << std::endl;
    std::cout << "\t " << died << " человек умерло с голоду, и " << newCitizens << " человек прибыли в наш великий город" << std::endl;
    if (wasPlague)
        std::cout << "\t Чума уничтожила половину населения" << std::endl;
    std::cout << "\t Население города сейчас составляет " << citizens << " человек" << std::endl;
    std::cout << "\t Мы собрали " << collectedWheat << " бушелей пшеницы, по " << wheatFromAcre << " бушеля с акра" << std::endl;
    std::cout << "\t Крысы истребили " << ratAte << " бушелей пшеницы, остаив " <<totalWheat <<" в амбарах"<<std::endl;
    std::cout << "\t Город сейчас занимает " << acres << " акров" << std::endl;
    std::cout << "\t 1 акр земли сейчас стоит " << acrePrice << " бушель" << std::endl;
    std::cout << std::endl;

}

void print_game_start(int citizens, int acres, int wheat, int acrePrice)
{
    std::cout << "\t Население города сейчас составляет " << citizens << " человек" << std::endl;
    std::cout << "\t В амбарах " << wheat << " бушелей пшеницы" << std::endl;
    std::cout << "\t Наши владения составляют " << acres << " акров" << std::endl;
    std::cout << "\t 1 акр земли сейчас стоит " << acrePrice << " бушель" << std::endl;
    std::cout << std::endl;
}

void ask_player(float& wheat, int& citizens, int& acres, int acrePrice, int& acresToSeed, int& wheatToEat)
{
    int newAcres = 0;
    int soldAcres = 0;

    std::cout << "Что пожелаешь, повелитель?" << std::endl;
    do
    {
        
        std::cout << "\tСколько акров земли повеливаешь купить? ";
        std::cin >> newAcres;
        std::cout << "\tСколько акров земли повеливаешь продать? ";
        std::cin >> soldAcres;
        std::cout << "\tСколько бушелей пшеницы повелеваешь съесть? ";
        std::cin >> wheatToEat;
        std::cout << "\tСколько акров земли повелеваешь засеять? ";
        std::cin >> acresToSeed;
    } while (! check_inputed_values(newAcres,soldAcres, wheatToEat, acresToSeed, wheat, citizens, acres,acrePrice));

    std::cout << std::endl;


    wheat -= wheatToEat;
    wheat += soldAcres * acrePrice;
    wheat -= newAcres * acrePrice;
    acres += newAcres;
    acres -= soldAcres;
    wheat -= acresToSeed * wheatForAcre;
    

}

void init_game_over()
{
    std::cout << "Game Over" << std::endl;
}

void show_game_results(float P, int L)
{
    if (P - 0.33 > epsilon || L < 7)
    {
        std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт и изгнал вас из города" << std::endl;
    }
    else if (P - 0.1 > epsilon || L < 9)
    {
        std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем" << std::endl;
    }
    else if (P - 0.03 > epsilon || L < 10)
    {
        std::cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова" << std::endl;
    }
    else
    {
        std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше" << std::endl;
    }

    std::cout << "P: " << P << " L: " << L;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    std::srand(time(0));

    int citizens=100;
    float wheat=2800;
    int acres=1000;
    int acresToSeed = 0;
    int wheatToEat = 0;
    int startYear = 1;
    int wheatFromAcre = 0;
    int wheatCollected = 0;
    int wheatAteByRats = 0;
    int died = 0;
    float diedPercents = 0;
    bool wasPlague = false;
    int newCitizens = 0;

    int acrePrice = rand() % 100 / acrePriceUpperBound + acrePriceLowerBound;
    bool wasSaved = false;


    

    float L = 0;
    int P = 0;


    
    
    if (is_file_exist("save.txt"))
        wasSaved=load_data(startYear,died,newCitizens,wasPlague,citizens,wheatCollected,wheatFromAcre,wheatAteByRats,wheat,acres,acrePrice,P,L);
    else
    {
        std::fstream fs;
        fs.open("save.txt", std::fstream::out);
        fs.close();
        
    }
    
    
    if (!wasSaved)
    {
        print_game_start(citizens, acres, wheat, acrePrice);
        ask_player(wheat, citizens, acres, acrePrice, acresToSeed, wheatToEat);
    }
    for (int i = startYear; i < rounds; i++)
    {
        if (!wasSaved)
        {
            wheatFromAcre = (rand() % collectedWheatUpperBound + collectedWheatLowerBound);
            wheatCollected = wheatFromAcre * clamp(citizens * citizenCultivationAcres, 0, acresToSeed);
            wheat += wheatCollected;
            wheatAteByRats = (int)(wheat * (float(rand() % 100) / 100 * ratAteUpperBound));
            wheat -= wheatAteByRats;

            died = clamp(citizens - (int)(wheatToEat / citizenWheatConsumption), 0, citizens);

            diedPercents = float(died) / float(citizens);
            L += diedPercents;

            if (diedCitizensForLose - float(died) / float(citizens) <= epsilon)
            {
                init_game_over();
                break;
            }

            citizens -= died;

            wasPlague = false;

            if (float(rand() % 100) / 100 <= 0.15f)
            {
                wasPlague = true;
                citizens = (int)citizens / 2;
            }

            newCitizens = count_new_citizens(died, wheatFromAcre, wheat);

            acrePrice = rand() % 100 / acrePriceUpperBound + acrePriceLowerBound;

            citizens += newCitizens;



            save_data(i, died, newCitizens, wasPlague, citizens, wheatCollected, wheatFromAcre, wheatAteByRats, wheat, acres, acrePrice,P,L);
        }
        wasSaved = false;
        print_game(i, died, newCitizens, wasPlague, citizens, wheatCollected, wheatFromAcre, wheatAteByRats, wheat, acres, acrePrice);
        ask_player(wheat,citizens,acres,acrePrice,acresToSeed,wheatToEat);
    }

    P = acres / citizens;
    L = L/rounds;

    show_game_results(L,P);
    clear_file();

    return 0;
}
