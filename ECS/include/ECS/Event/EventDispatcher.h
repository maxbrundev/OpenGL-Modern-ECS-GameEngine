#pragma once

#include "ecs_stdafx.h"

#include "IEventDispatcher.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace ECS 
{
	namespace Event 
	{
		template<typename T>
		class EventDispatcher : public IEventDispatcher
		{
		private:
			std::list<IEventDelegate*>	m_pendingRemoveDelegates;
			std::list<IEventDelegate*>	m_eventCallbacks;
			bool						m_isLocked;

		public:
			EventDispatcher() : m_isLocked(false)
			{}

			virtual ~EventDispatcher()
			{
				this->m_pendingRemoveDelegates.clear();
				this->m_eventCallbacks.clear();
			}

			inline void Dispatch(const IEvent* p_event) override
			{
				this->m_isLocked = true;
				{
//					LOG(Info(), "Dispatch event " << typeid(T).name());

					if (!this->m_pendingRemoveDelegates.empty())
					{
						for (auto callback : this->m_pendingRemoveDelegates)
						{
							auto result = std::find_if(this->m_eventCallbacks.begin(), this->m_eventCallbacks.end(),
								[&](const IEventDelegate* other)
							{
								return (*other) == callback;
							});

							
							if (result != this->m_eventCallbacks.end())
							{
								this->m_eventCallbacks.erase(result);
							}
						}
						this->m_pendingRemoveDelegates.clear();
					}

					for (auto callback : this->m_eventCallbacks)
					{
						ASSERT(callback != nullptr,"Invalid event callback.");
						callback->Invoke(p_event);
					}
				}
				this->m_isLocked = false;
			}

			virtual void AddEventCallback(IEventDelegate* const p_eventDelegate) override
			{
				auto result = std::find_if(this->m_pendingRemoveDelegates.begin(), this->m_pendingRemoveDelegates.end(),
					[&](const IEventDelegate* other)
				{
					return (*other) == p_eventDelegate;
				});

				if (result != this->m_pendingRemoveDelegates.end())
				{
					this->m_pendingRemoveDelegates.erase(result);
					return;
				}

				this->m_eventCallbacks.push_back(p_eventDelegate);
			}

			virtual void RemoveEventCallback(IEventDelegate* p_eventDelegate) override
			{
				if (!this->m_isLocked)
				{
					auto result = std::find_if(this->m_eventCallbacks.begin(), this->m_eventCallbacks.end(),
						[&](const IEventDelegate* other)
					{
						return (*other) == p_eventDelegate;
					});

					if (result != this->m_eventCallbacks.end())
						this->m_eventCallbacks.erase(result);
				}
				else
				{
					auto result = std::find_if(this->m_eventCallbacks.begin(), this->m_eventCallbacks.end(),
						[&](const IEventDelegate* other)
					{
						return (*other) == p_eventDelegate;
					});

					if(result != this->m_eventCallbacks.end())
						this->m_pendingRemoveDelegates.push_back((*result));
				}
			}

			virtual inline size_t GetEventCallbackCount() const override { return this->m_eventCallbacks.size(); }
		};
	}
}

#pragma warning(pop)