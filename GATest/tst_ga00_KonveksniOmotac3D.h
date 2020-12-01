#ifndef TST_GA00_KONVEKSNIOMOTAC3D_H
#define TST_GA00_KONVEKSNIOMOTAC3D_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <unordered_set>

#include "./ga04_konveksniomotac3d.h"

using namespace testing;


TEST(ga04_konveksniomotac3d, firstLERandomTest)
{
    KonveksniOmotac3D* ch1 = new KonveksniOmotac3D(nullptr, 0, "", 30);
    ch1->pokreniAlgoritam();
    ch1->pokreniNaivniAlgoritam();
    EXPECT_LE(ch1->getIvice().size(), ch1->getNaivneIvice().size());
}

TEST(ga04_konveksniomotac3d, secondLERandomTest)
{
    KonveksniOmotac3D* ch1 = new KonveksniOmotac3D(nullptr, 0, "", 100);
    ch1->pokreniAlgoritam();
    ch1->pokreniNaivniAlgoritam();
    EXPECT_LE(ch1->getIvice().size(), ch1->getNaivneIvice().size());
}

TEST(ga04_konveksniomotac3d, firstAdvancedTestFromFile)
{
    KonveksniOmotac3D* ch1 = new KonveksniOmotac3D(nullptr, 0, "../input_KonveksniOmotac3D.txt", 0);
    ch1->pokreniAlgoritam();
    ch1->pokreniNaivniAlgoritam();

    std::unordered_set<Teme*> temena;
    std::unordered_set<Teme*> naivnaTemena;

    for(auto ivica : ch1->getIvice())
    {
        temena.insert(ivica->t1());
        temena.insert(ivica->t2());
    }

    for(auto naivnaIvica : ch1->getNaivneIvice())
    {
        naivnaTemena.insert(naivnaIvica->t1());
        naivnaTemena.insert(naivnaIvica->t2());
    }

    EXPECT_EQ(naivnaTemena.size(), temena.size());
}

TEST(ga04_konveksniomotac3d, firstAdvancedRandomTest)
{
    KonveksniOmotac3D* ch1 = new KonveksniOmotac3D(nullptr, 0, "", 100);
    ch1->pokreniAlgoritam();
    ch1->pokreniNaivniAlgoritam();

    std::unordered_set<Teme*> temena;
    std::unordered_set<Teme*> naivnaTemena;

    for(auto ivica : ch1->getIvice())
    {
        temena.insert(ivica->t1());
        temena.insert(ivica->t2());
    }

    for(auto naivnaIvica : ch1->getNaivneIvice())
    {
        naivnaTemena.insert(naivnaIvica->t1());
        naivnaTemena.insert(naivnaIvica->t2());
    }

    EXPECT_EQ(naivnaTemena.size(), temena.size());
}

#endif // TST_GA00_KONVEKSNIOMOTAC3D_H
