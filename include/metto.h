#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<nec.h>

#define METTO_PROTOCOL 0
#define SHIFT_PROTOCOL 1
#define TIME_PAD_PROTOCOL 2

class metto
{
private:
    int protocol;
    nec<char> characters;
    nec<char> scrambled;
    
    int Find(nec<char> *cs, char c)
    {
        for(int i = 0; i < cs->size; i++)
            if((*cs)[i] == c) return i;
    }
    int GenerateCharacters(int shift = 0)
    {
        if(shift == 0)
        {
            srand(time(0));
            shift = rand() % 20 + 1;
        }
        int offset = shift;
        
        characters << '\n';
        for(unsigned char c = 32; c < 127; c++) characters << c;

        for(int i = 0; i < characters.size; i++)
        {
            if(i + offset >= characters.size) offset = -i;
            scrambled << characters[i + offset];
        }

        return shift;
    }

    std::string GenerateKey(std::string *buffer)
    {
        srand(time(0));
        std::string password;
        for(int i = 0; i < buffer->size(); i++)
        {
            reload:
            int key = rand() % characters.size - Find(&characters, (*buffer)[i]);
            if(key < 0) goto reload;
            password += characters[key];
            (*buffer)[i] = characters[Find(&characters, characters[key]) + Find(&characters, (*buffer)[i])];
        }

        return password;
    }
    void Unlock(std::string *buffer)
    {
        int offset = (buffer->size() >> 1);
        for(int i = 0; i < offset; i++)
        {
            char key = (*buffer)[offset];
            (*buffer)[i] = characters[Find(&characters, (*buffer)[i]) - Find(&characters, key)];
            buffer->erase(buffer->begin() + offset);
        }
    }
public:
    std::string Scramble(std::string buffer)
    {
        int shift = GenerateCharacters();

        if(protocol == TIME_PAD_PROTOCOL || protocol == METTO_PROTOCOL)
        {
            std::string key = GenerateKey(&buffer);
            buffer.append(key);
        }

        if(protocol == SHIFT_PROTOCOL || protocol == METTO_PROTOCOL)
        {
            std::string liner = "seha";
            buffer.append(liner);
            for(int i = 0; i < buffer.size(); i++) buffer[i] = scrambled[Find(&characters, buffer[i])];
        }

        return buffer;
    }
    std::string Descramble(std::string buffer)
    {
        if(protocol == SHIFT_PROTOCOL || protocol == METTO_PROTOCOL)
        {
            std::string liner;
            for(int i = buffer.size() - 4; i < buffer.size(); i)
            {
                liner += buffer[i];
                buffer.erase(buffer.begin() + i);
            }

            int shift = 1;
            while(true)
            {
                std::string checker = liner;
                GenerateCharacters(shift++);
                for(int i = 0; i < checker.size(); i++) checker[i] = characters[Find(&scrambled, checker[i])];
                if(checker == "seha") break;
                characters.RemoveAll();
                scrambled.RemoveAll();
            }

            for(int i = 0; i < buffer.size(); i++) buffer[i] = characters[Find(&scrambled, buffer[i])];
        }

        if(protocol == TIME_PAD_PROTOCOL || protocol == METTO_PROTOCOL)
        {
            if(protocol == TIME_PAD_PROTOCOL) GenerateCharacters();
            Unlock(&buffer);
        }

        return buffer;
    }

    metto(int _protocol) { protocol = _protocol; }
};