// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/InputBuffer.h" 

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

FString InputToString(EInputType type)
{
    switch (type)
    {
    case EInputType::None:
        return "None";
    case EInputType::Top:
        return "Top";
    case EInputType::TopRight:
        return "TopRight";
    case EInputType::Right:
        return "Right";
    case EInputType::DownRight:
        return "DownRight";
    case EInputType::Down:
        return "Down";
    case EInputType::DownLeft:
        return "DownLeft";
    case EInputType::Left:
        return "Left";
    case EInputType::TopLeft:
        return "TopLeft";
    case EInputType::LightPunch:
        return "LightPunch";
    case EInputType::MediumPunch:
        return "MediumPunch";
    case EInputType::HeavyPunch:
        return "HeavyPunch";
    case EInputType::LightKick:
        return "LightKick";
    case EInputType::MediumKick:
        return "MediumKick";
    case EInputType::HeavyKick:
        return "HeavyKick";
    case EInputType::Block:
        return "Block";
    default:
        return "None";
    }
}

EInputType InputFromString(FString string)
{
    if (string == "None")
        return EInputType::None;
    if (string == "Top")
        return EInputType::Top;
    if (string == "TopRight")
        return EInputType::TopRight;
    if (string == "Right")
        return EInputType::Right;
    if (string == "DownRight")
        return EInputType::DownRight;
    if (string == "Down")
        return EInputType::Down;
    if (string == "DownLeft")
        return EInputType::DownLeft;
    if (string == "Left")
        return EInputType::Left;
    if (string == "TopLeft")
        return EInputType::TopLeft;
    if (string == "LightPunch")
        return EInputType::LightPunch;
    if (string == "MediumPunch")
        return EInputType::MediumPunch;
    if (string == "LightPunch")
        return EInputType::LightPunch;
    if (string == "MediumPunch")
        return EInputType::MediumPunch;
    if (string == "HeavyPunch")
        return EInputType::HeavyPunch;
    if (string == "LightKick")
        return EInputType::LightKick;
    if (string == "MediumKick")
        return EInputType::MediumKick;
    if (string == "HeavyKick")
        return EInputType::HeavyKick;
    if (string == "Block")
        return EInputType::Block;

    return EInputType::None;
}

InputBuffer::InputBuffer()
{
}

InputBuffer::InputBuffer(TArray<MotionInput*>& inputs)
{
    m_MotionInputs = inputs;

    /*UEnhancedInputLocalPlayerSubsystem sus;

    sus.GetPlayerInput()->ActionMappings[0].*/

    //UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    //InputBufferItem* item = new InputBufferItem();

    ////Input->BindAction(inputActions[0], ETriggerEvent::Triggered, item, &InputBufferItem::InputCheck);

    //m_MotionInputs = inputs;

    //Initialize();

    //ControlInputs->GetPlayerInput()->ActionMappings[0].Key = FKey::press

}

InputBuffer::~InputBuffer()
{
}

void InputBuffer::add(TArray<MotionInput*>& inputs)
{
    m_MotionInputs = inputs;

    /*UEnhancedInputLocalPlayerSubsystem sus;

    sus.GetPlayerInput()->ActionMappings[0].*/

    //UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

    //InputBufferItem* item = new InputBufferItem();

    ////Input->BindAction(inputActions[0], ETriggerEvent::Triggered, item, &InputBufferItem::InputCheck);

    //m_MotionInputs = inputs;

    //Initialize();

    //ControlInputs->GetPlayerInput()->ActionMappings[0].Key = FKey::press

}

void InputBuffer::Initialize()
{
    //InputBufferItems.Empty();

    //for(int i = 0; i < ControlInputs->GetPlayerInput()->ActionMappings.Num(); i++)
    //{
    //   /* InputBufferItem* item = new InputBufferItem();

    //    item->AssignButton(ControlInputs->GetPlayerInput()->ActionMappings[i]);

    //    InputBufferItems.push_back(item);*/
    //}
}

void InputBuffer::BufferUpdate()
{
    if (m_InputBufferItems.Num() > 0)
    {
        for (auto bufferItem : m_InputBufferItems)
        {
            // Checks if one of the buttons is pressed
            bufferItem->InputCheck();

            // This reads the buffer in reverse
            //for(int i = 0; i < bufferItem.Buffer.Count - 1; i++)
            //{
            //    // Moves the buffer data higher
            //    bufferItem.SetHoldUsed(i, bufferItem.Buffer[i + 1].HoldTime, bufferItem.Buffer[i + 1].IsUsed);
            //}

            if (bufferItem->m_Buffer.Num() > 1)
            {
                for (int i = bufferItem->m_Buffer.Num() - 1; i > 0; i--)
                {
                    // Moves the buffer data higher
                    bufferItem->SetHoldUsed(i, bufferItem->m_Buffer[i - 1].HoldTime, bufferItem->m_Buffer[i - 1].IsUsed, bufferItem->m_Buffer[i - 1].MotionUsed);
                }
            }

           /* if (bufferItem->InputDirection == EInputType::LightPunch)
            {
                GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::FromInt(bufferItem->m_Buffer[0].HoldTime));
                GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::FromInt(bufferItem->m_Buffer[0].IsUsed));
            }*/
        }
    }
}

void InputBuffer::UpdateMotion(bool right)
{
    for (int i = 0; i < m_MotionInputs.Num(); i++)
    {
        m_MotionInputs[i]->BufferCheck();

        for (int j = 0; j < m_InputBufferItems.Num(); j++)
        {
            if (m_InputBufferItems[j]->m_Buffer.Num() > 0)
            {
                if (m_InputBufferItems[j]->m_Buffer[0].CanMotionExecute())
                {
                    EInputType direction = EInputType::None;

                    switch (m_InputBufferItems[j]->InputDirection)
                    {
                    case EInputType::Right:
                        /* if(right)*/
                        direction = EInputType::Right;
                        /*else if(!right)
                            direction = EInputType::Left;*/
                        break;
                    case EInputType::Left:
                        /*if (right)*/
                        direction = EInputType::Left;
                        /*else if (!right)
                            direction = EInputType::Right;*/
                        break;
                    case EInputType::LightPunch:
                        direction = EInputType::LightPunch;
                        break;
                    case EInputType::MediumPunch:
                        direction = EInputType::MediumPunch;
                        break;
                    case EInputType::HeavyPunch:
                        direction = EInputType::HeavyPunch;
                        break;
                    }

                    if (m_MotionInputs[i]->InputCheck(direction))
                    {
                        m_InputBufferItems[j]->m_Buffer[1].SetMotionTrue();
                    }
                }
            }
        }

        if (m_MotionInputs[i]->MotionComplete() == true)
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Motion completed"));
        }
    }
}

void InputStateItem::HoldUp()
{
    if (HoldTime < 0)
    {
        HoldTime = 1;
    }
    else
    {
        HoldTime += 1;

        if (HoldTime > 99)
            HoldTime = 99;
    }
}

void InputStateItem::ReleasedUp()
{
    if (HoldTime > 0)
    {
        HoldTime = -1;

        IsUsed = false;
        MotionUsed = false;
    }
    else
    {
        HoldTime = 0;
    }
}

void InputStateItem::SetUsedTrue()
{
    IsUsed = true;
}

void InputStateItem::SetMotionTrue()
{
    MotionUsed = true;
}

void InputStateItem::SetHoldUsed(int time, bool used, bool motion)
{
    HoldTime = time;

    IsUsed = used;

    MotionUsed = motion;
}

bool InputStateItem::CanExecute()
{
    if (HoldTime == 1 && !IsUsed)
    {
        return true;
    }

    return false;
}

bool InputStateItem::CanMotionExecute()
{
    if (HoldTime == 1 && !MotionUsed)
    {
        return true;
    }

    return false;
}

InputBufferItem::InputBufferItem()
{
    for (int i = 0; i < 12; i++)
    {
        m_Buffer.Add(InputStateItem());
    }
}

InputBufferItem::InputBufferItem(EInputType direction)
{
    InputDirection = direction;
}

void InputBufferItem::AssignDirection(EInputType direction)
{
    InputDirection = direction;
}

void InputBufferItem::InputCheck()
{
    if (m_Buffer.Num() > 0)
    {
        if (m_InputActionPressed)
        {
            m_Buffer[0].HoldUp();
        }
        else
        {
            m_Buffer[0].ReleasedUp();
        }
    }
}

void InputBufferItem::SetUsedTrue()
{
    m_Used = true;
}

void InputBufferItem::SetUsedFalse()
{
    m_Used = false;
}

void InputBufferItem::SetInputActionPressed(bool pressed)
{
    m_InputActionPressed = pressed;

    if (pressed == 0)
        SetUsedFalse();
}

void InputBufferItem::SetHoldUsed(int index, int time, bool used, bool motion)
{
    m_Buffer[index].SetHoldUsed(time, used, motion);
}

MotionInput::MotionInput()
{
}

MotionInput::MotionInput(TArray<EInputType> motions)
{
    MotionInputs = motions;
}

void MotionInput::BufferCheck()
{
    if (IndexCheck > 0)
    {
        BufferTime++;

        if (BufferTime > 12)
            ResetInput();
    }
    else
        BufferTime = 0;
}

void MotionInput::IncreaseIndex()
{
    IndexCheck++;

    BufferTime = 0;
}

void MotionInput::ResetInput()
{
    IndexCheck = 0;

    BufferTime = 0;
}

bool MotionInput::MotionComplete()
{
    if (IndexCheck >= MotionInputs.Num() && BufferTime <= 12)
    {
        BufferTime = 12;

        return true;
    }

    return false;
}

bool MotionInput::InputCheck(EInputType input)
{
    if (IndexCheck < MotionInputs.Num())
    {
        if (MotionInputs[IndexCheck] == input)
        {
            IncreaseIndex();

            return true;
        }
    }

    return false;
}
