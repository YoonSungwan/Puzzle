// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubject.h"

UGameStateSubject::UGameStateSubject()
{
	PlayerScore = 0;
}

void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Add(Observer);
}

void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Remove(Observer);
}

//점수 변화 시, 모든 Observer에게 알림
void UGameStateSubject::NotifyObservers()
{
	UE_LOG(LogTemp, Warning, TEXT("UObserver::NotifyObservers"));
	for (TScriptInterface<IObserver> Observer : Observers)
	{
		//Observer 객체가 유효 && IObserver 인터페이스를 구현하고(가지고 있는지) 있는지 확인
		if(Observer.GetObject() && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
		{
			//Observer의 OnNotify 함수를 호출
			//실질적인 클래스의 인스턴스에서 함수를 호출하는 방식에서 -> 대신 인터페이스를 호출(의존성 낮추고 디커플링)
			UE_LOG(LogTemp, Warning, TEXT("UObserver::NotifyObservers %d"), PlayerScore);
			IObserver::Execute_OnNotify(Observer.GetObject(), PlayerScore);
		}
	}
}

void UGameStateSubject::IncreaseScore(int32 Increament)
{
	PlayerScore += Increament;

	//점수가 변경되면 알려줘야함 
	NotifyObservers();
}
