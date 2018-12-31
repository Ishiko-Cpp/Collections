/*
    Copyright (c) 2018 Xavier Leclercq

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "ObservableVectorTests.h"

using namespace Ishiko::TestFramework;

void ObservableVectorTests::AddTests(TestHarness& theTestHarness)
{
    TestSequence& vectorTestSequence = theTestHarness.appendTestSequence("ObservableVector tests");

    new HeapAllocationErrorsTest("Creation test 1", CreationTest1, vectorTestSequence);

    new HeapAllocationErrorsTest("pushBack test 1", PushBackTest1, vectorTestSequence);
    new HeapAllocationErrorsTest("pushBack test 2", PushBackTest2, vectorTestSequence);
    new HeapAllocationErrorsTest("pushBack test 3", PushBackTest3, vectorTestSequence);

    new HeapAllocationErrorsTest("erase test 1", EraseTest1, vectorTestSequence);
    new HeapAllocationErrorsTest("erase test 2", EraseTest2, vectorTestSequence);
    new HeapAllocationErrorsTest("erase test 3", EraseTest3, vectorTestSequence);

    new HeapAllocationErrorsTest("Two observers test 1", TwoObserversTest1, vectorTestSequence);

    new HeapAllocationErrorsTest("The same observer twice test 1", TheSameObserverTwiceTest1, vectorTestSequence);

    new HeapAllocationErrorsTest("Observer removal test 1", ObserverRemovalTest1, vectorTestSequence);
    new HeapAllocationErrorsTest("Observer removal test 2", ObserverRemovalTest2, vectorTestSequence);
    new HeapAllocationErrorsTest("Observer removal test 3", ObserverRemovalTest3, vectorTestSequence);

    new HeapAllocationErrorsTest("Observer deletion test 1", ObserverDeletionTest1, vectorTestSequence);

    new HeapAllocationErrorsTest("Two vectors sharing an observer test 1", TwoVectorsTest1, vectorTestSequence);
}

TestResult::EOutcome ObservableVectorTests::CreationTest1()
{
    Ishiko::Collections::ObservableVector<int> vector;
    if (vector.size() == 0)
    {
        return TestResult::ePassed;
    }
    else
    {
        return TestResult::eFailed;
    }
}

TestResult::EOutcome ObservableVectorTests::PushBackTest1()
{
    Ishiko::Collections::ObservableVector<int> vector;
    vector.pushBack(123);
    if ((vector.size() == 1) && (vector[0] == 123))
    {
        return TestResult::ePassed;
    }
    else
    {
        return TestResult::eFailed;
    }
}

TestResult::EOutcome ObservableVectorTests::PushBackTest2()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);
    if ((vector.size() == 1) && (vector[0] == 123))
    {
        if ((observer->m_additions[&vector].size() == 1) &&
            (observer->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::PushBackTest3()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);
    vector.pushBack(456);
    vector.pushBack(789);
    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((observer->m_additions[&vector].size() == 3) &&
            (observer->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)) &&
            (observer->m_additions[&vector][1] == std::pair<size_t, int>(1, 456)) &&
            (observer->m_additions[&vector][2] == std::pair<size_t, int>(2, 789)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::EraseTest1()
{
    Ishiko::Collections::ObservableVector<int> vector;
    vector.pushBack(123);
    vector.erase(vector.begin());
    if (vector.size() == 0)
    {
        return TestResult::ePassed;
    }
    else
    {
        return TestResult::eFailed;
    }
}

TestResult::EOutcome ObservableVectorTests::EraseTest2()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);
    vector.erase(vector.begin());

    if (vector.size() == 0)
    {
        if ((observer->m_deletions.size() == 1) &&
            (observer->m_deletions[&vector][0] == std::tuple<size_t, size_t, std::vector<int>>(0, 1, { 123 })))
        {
            result = TestResult::ePassed;
        }
    }
    
    return result;
}

TestResult::EOutcome ObservableVectorTests::EraseTest3()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);
    vector.pushBack(456);
    vector.pushBack(789);
    vector.erase(vector.begin() + 1, vector.end());

    if (vector.size() == 1)
    {
        if ((observer->m_deletions.size() == 1) &&
            (observer->m_deletions[&vector][0] == std::tuple<size_t, size_t, std::vector<int>>(1, 3, { 456, 789 })))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::TwoObserversTest1()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer1 = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer1);
    std::shared_ptr<IntVectorObserver> observer2 = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer2);

    vector.pushBack(123);
    vector.pushBack(456);
    vector.pushBack(789);
    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((observer1->m_additions[&vector].size() == 3) &&
            (observer1->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)) &&
            (observer1->m_additions[&vector][1] == std::pair<size_t, int>(1, 456)) &&
            (observer1->m_additions[&vector][2] == std::pair<size_t, int>(2, 789)) &&
            (observer2->m_additions[&vector].size() == 3) &&
            (observer2->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)) &&
            (observer2->m_additions[&vector][1] == std::pair<size_t, int>(1, 456)) &&
            (observer2->m_additions[&vector][2] == std::pair<size_t, int>(2, 789)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::TheSameObserverTwiceTest1()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);
    vector.observers().add(observer);

    vector.pushBack(123);
    vector.pushBack(456);
    vector.pushBack(789);
    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((observer->m_additions[&vector].size() == 3) &&
            (observer->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)) &&
            (observer->m_additions[&vector][1] == std::pair<size_t, int>(1, 456)) &&
            (observer->m_additions[&vector][2] == std::pair<size_t, int>(2, 789)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::ObserverRemovalTest1()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer1 = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer1);
    std::shared_ptr<IntVectorObserver> observer2 = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer2);

    vector.pushBack(123);

    vector.observers().remove(observer2);

    vector.pushBack(456);
    vector.pushBack(789);

    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((observer1->m_additions[&vector].size() == 3) &&
            (observer1->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)) &&
            (observer1->m_additions[&vector][1] == std::pair<size_t, int>(1, 456)) &&
            (observer1->m_additions[&vector][2] == std::pair<size_t, int>(2, 789)) &&
            (observer2->m_additions[&vector].size() == 1) &&
            (observer2->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::ObserverRemovalTest2()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);
    vector.observers().add(observer);

    vector.pushBack(123);

    vector.observers().remove(observer);

    vector.pushBack(456);

    vector.observers().remove(observer);

    vector.pushBack(789);

    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((observer->m_additions[&vector].size() == 2) &&
            (observer->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)) &&
            (observer->m_additions[&vector][1] == std::pair<size_t, int>(1, 456)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::ObserverRemovalTest3()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);
    
    vector.pushBack(123);

    vector.observers().remove(observer);
    vector.observers().remove(observer);

    vector.pushBack(456);

    if ((vector.size() == 2) && (vector[0] == 123) && (vector[1] == 456))
    {
        if ((observer->m_additions[&vector].size() == 1) &&
            (observer->m_additions[&vector][0] == std::pair<size_t, int>(0, 123)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::ObserverDeletionTest1()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector.observers().add(observer);

    vector.pushBack(123);

    std::vector<std::pair<size_t, int>> additions(observer->m_additions[&vector]);

    observer.reset();

    vector.pushBack(456);
    vector.pushBack(789);

    if ((vector.size() == 3) && (vector[0] == 123) && (vector[1] == 456) && (vector[2] == 789))
    {
        if ((additions.size() == 1) &&
            (additions[0] == std::pair<size_t, int>(0, 123)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

TestResult::EOutcome ObservableVectorTests::TwoVectorsTest1()
{
    TestResult::EOutcome result = TestResult::eFailed;

    Ishiko::Collections::ObservableVector<int> vector1;
    Ishiko::Collections::ObservableVector<int> vector2;

    std::shared_ptr<IntVectorObserver> observer = std::make_shared<IntVectorObserver>();
    vector1.observers().add(observer);
    vector2.observers().add(observer);

    vector1.pushBack(123);
    vector2.pushBack(456);

    if ((vector1.size() == 1) && (vector1[0] == 123))
    {
        if ((observer->m_additions[&vector1].size() == 1) &&
            (observer->m_additions[&vector1][0] == std::pair<size_t, int>(0, 123)) &&
            (observer->m_additions[&vector2].size() == 1) &&
            (observer->m_additions[&vector2][0] == std::pair<size_t, int>(0, 456)))
        {
            result = TestResult::ePassed;
        }
    }

    return result;
}

void IntVectorObserver::onElementAdded(const Ishiko::Collections::ObservableVector<int>& source, size_t pos,
    const int& value)
{
    m_additions[(void*)&source].push_back(std::pair<size_t, int>(pos, value));
}

void IntVectorObserver::onElementsRemoved(const Ishiko::Collections::ObservableVector<int>& source, size_t first,
    size_t last, const std::vector<int>& removedElements)
{
    m_deletions[(void*)&source].push_back(std::tuple<size_t, size_t, std::vector<int>>(first, last, removedElements));
}
