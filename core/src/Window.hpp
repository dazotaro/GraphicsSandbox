/*
 * Window.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jusabiaga
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

namespace JU
{

class Window
{
    public:
        Window ();
        virtual ~Window ();

        bool initialize();
        void exit();

};

} /* namespace JU */

#endif /* WINDOW_HPP_ */
