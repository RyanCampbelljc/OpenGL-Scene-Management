#include "../samplefw/SampleRunner.h"
#include "../wolf/wolf.h"
#include "DebugSample.h"
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>

class Week2 : public wolf::App
{
public:
    Week2()
        : wolf::App("City")
    {
        // INIT ALL SAMPLES
        m_sampleRunner.addSample(new DebugSample(this));
    }

    ~Week2()
    {
    }

    void update(float dt) override
    {
        if (isKeyDown(' '))
        {
            m_lastDown = true;
        }
        else if (m_lastDown)
        {
            // IF YOU WANT TO SWITCH SAMPLES
            // m_sampleRunner.nextSample();
            m_lastDown = false;
        }

        m_sampleRunner.update(dt);
    }

    void render() override
    {
        m_sampleRunner.render(m_width, m_height);
    }

private:
    SampleRunner m_sampleRunner;
    bool m_lastDown = false;
};

int main(int, char** argv)
{
    std::cout << argv[0] << std::endl;
    Week2 week2;
    week2.run();
}