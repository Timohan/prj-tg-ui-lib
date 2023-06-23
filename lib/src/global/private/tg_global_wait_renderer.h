/*!
 * \file
 * \brief file tg_global_wait_renderer.h
 *
 * wait renderer
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#ifndef TG_GLOBAL_WAIT_RENDER_H
#define TG_GLOBAL_WAIT_RENDER_H

#include <mutex>

#ifndef DEFAULT_RENDER_WAIT_MAX_TIMEOUT
#define DEFAULT_RENDER_WAIT_MAX_TIMEOUT 1000
#endif

class TgGlobalWaitRenderer
{
public:
    static TgGlobalWaitRenderer *getInstance();
    void waitForRender();
    void release(size_t maxTimeout = UINT64_MAX);

private:
    static TgGlobalWaitRenderer *m_globalWaitRender;
    std::timed_mutex m_semHandler;
    size_t m_nextTimeMaxTimeOut = 0;
    size_t m_currentTimeMaxTimeOut = DEFAULT_RENDER_WAIT_MAX_TIMEOUT;
    size_t m_renderCount = 0;
    std::mutex m_mutex;
    bool m_semLocked = false;
};

#endif // TG_GLOBAL_WAIT_RENDER_H