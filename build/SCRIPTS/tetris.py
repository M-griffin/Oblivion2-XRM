""" Tetris game for x/84. """
__author__ = 'Johannes Lundberg'
__copyright__ = 'Copyright 2007-2013'
__license__ = 'Public Domain'
# Single player tetris, originally written for The Progressive (prsv), the
# predecessor python bbs system of x/84.
# Copyright (C) 2007-2013 Johannes Lundberg

import bbs_io
import gc
from select import select
from sys import stdin


def main():
    #from x84.bbs import getsession, getterminal, echo
    #session, term = getsession(), getterminal()
    #session.activity = 'playing tetris'


    while True:
        play()
    
        # Testing the pyGetKey Functions
        bbs_io.pyPipe2Ansi('|CS|CR|11Tetris was ported in python from x/84 for Enthral BBS.')
        bbs_io.pyPipe2Ansi('|CR|07Thanks to x/84 for open code, check out |15telnet://1984.ws')
        bbs_io.pyPipe2Ansi('|CR|03Highscores are in the works.')
        bbs_io.pyPipe2Ansi('|CR|CR|09pLay aGain? [y/n]')

        # ch = the int key code returned from key press.
        while True:
            ch = bbs_io.pyGetKey()
            ch = chr(int(ch))
        
            if ch in ('y','Y'):
                break
            elif ch in ('n','N'):
                return
            
    
    #    if score[0] > 0:
    #        register_score(session.user.handle, score)
    #    show_scores()


#def register_score(handle, score):
#    from x84.bbs import DBProxy
#    db = DBProxy('tetris')
#    if not handle in db:
#        db[handle] = score
#    elif score[0] > db[handle][0]:
#        db[handle] = score


#def show_scores():
#    from x84.bbs import DBProxy, Pager, getterminal
#    from x84.bbs import getch, echo, getsession, ini
#    session, term = getsession(), getterminal()
#    allscores = DBProxy('tetris').items()
#    if 0 == len(allscores):
#        return
#    # line up over tetris game, but logo & 'made by jojo' in view
#    # -- since we have so much screen width, columize the scores,
#    # the math brings it out to 2 columns, but fmt is adjustable
#    pager_title = term.blue_reverse_underline('- hiGh SCOREs -')
#    len_handle = ini.CFG.getint('nua', 'max_user')
#    score_fmt = u'%s %s %s %s'
#    len_pos = 2
#    len_score = 10
#    len_level = 3
#    height, width = 11, 73
#    yloc, xloc = 10, 3
#    pager = Pager(height=height, width=width, yloc=yloc, xloc=xloc)
#    pager.xpadding = 1
#    pager.glyphs['left-vert'] = u''
#    pager.glyphs['right-vert'] = u''
#    pager.colors['border'] = term.blue_reverse
#    pager.alignment = 'center'
#    # pre-fesh pager border before fetch
#    echo(pager.border() + pager.title(pager_title) + pager.clear())
#    highscores = sorted(
#        [(_score, _level, _handle.decode('utf8'))
#         for (_handle, (_score, _level, _)) in allscores],
#        reverse=True)
#    pager.append(score_fmt % (
#        term.bold_blue_underline('No'.ljust(len_pos)),
#        term.bold_blue_underline('SCORE'.ljust(len_score)),
#        term.bold_blue_underline('lVl'.ljust(len_level)),
#        term.bold_blue_underline('hANdlE'.rjust(len_handle),)))
#    pos = 0
#    for pos, (_score, _level, _handle) in enumerate(highscores):
#        if _handle == session.user.handle:
#            pager.append(score_fmt % (
#                term.blue_reverse(str(pos + 1).ljust(len_pos)),
#                term.blue_reverse(str(_score).ljust(len_score)),
#                term.blue_reverse(str(_level).ljust(len_level)),
#                term.blue_reverse(_handle.rjust(len_handle)),))
#        else:
#            pager.append(score_fmt % (
#                term.bold_blue(str(pos + 1).ljust(len_pos)),
#                term.blue(str(_score).ljust(len_score)),
#                term.blue(str(_level).ljust(len_level)),
#                _handle.rjust(len_handle),))
#    # append additional empty slot rows
#    while len(pager.content) < pager.visible_height:
#        pos += 1
#        pager.append(score_fmt % (
#            term.bold_blue(str(pos + 1).ljust(len_pos)),
#            term.bold_black(u'.'.ljust(len_score)),
#            term.bold_black(u'.'.ljust(len_level)),
#            term.bold_black(u'.'.rjust(len_handle)),))
#
#    dirty = 2  # 2=do not refresh border & title
#    pager.move_home()
#    while not pager.quit:
#        # 1=full refresh
#        dirty = 1 if session.poll_event('refresh') else dirty
#        if dirty:
#            echo(u''.join((
#                term.normal,
#                ((pager.border() + pager.title(pager_title))
#                    if dirty != 2 else u''),
#                pager.refresh(),
#                pager.footer(u''.join((
#                    term.underline_blue('q'),
#                    term.bold_blue('uit')))),
#            )))
#            dirty = 0
#        echo(pager.process_keystroke(getch(1)))


def play():
    import time
    from random import randint
    import os
    #from x84.bbs import getterminal, getch, from_cp437, AnsiWindow, syncterm_setfont
    #from x84.bbs import echo as echo_unbuffered
    #term = getterminal()
    field = []
    global charcache
    charcache = ''
    field_width = 10
    field_height = 20
    # Access scheme looks like this:
    #   layout[p][r][ypox][xpos]
    # layoutcolor = [ 7,2,3,4,4,6,7 ]
    layout = [
        #  ##
        #  ##
        [
            [
                [1, 1, ],
                [1, 1, ],
            ],
        ],
        #  #
        #  #
        #  #
        #  #
        [
            [
                [0, 1, 0, 0],
                [0, 1, 0, 0],
                [0, 1, 0, 0],
                [0, 1, 0, 0],
            ],
            [
                [0, 0, 0, 0],
                [1, 1, 1, 1],
                [0, 0, 0, 0],
                [0, 0, 0, 0],
            ]
        ],
        #  ###
        #   #
        [
            [
                [0, 0, 0],
                [1, 1, 1],
                [0, 1, 0],
            ],
            [
                [0, 1, 0],
                [0, 1, 1],
                [0, 1, 0],
            ],
            [
                [0, 1, 0],
                [1, 1, 1],
                [0, 0, 0],
            ],
            [
                [0, 1, 0],
                [1, 1, 0],
                [0, 1, 0],
            ],
        ],
        #  #
        #  #
        #  ##
        [
            [
                [0, 1, 0],
                [0, 1, 0],
                [0, 1, 1],
            ],
            [
                [0, 0, 1],
                [1, 1, 1],
                [0, 0, 0],
            ],
            [
                [1, 1, 0],
                [0, 1, 0],
                [0, 1, 0],
            ],
            [
                [0, 0, 0],
                [1, 1, 1],
                [1, 0, 0],
            ],
        ],
        #   #
        #   #
        #  ##
        [
            [
                [0, 1, 0],
                [0, 1, 0],
                [1, 1, 0],
            ],
            [
                [0, 0, 0],
                [1, 1, 1],
                [0, 0, 1],
            ],
            [
                [0, 1, 1],
                [0, 1, 0],
                [0, 1, 0],
            ],
            [
                [1, 0, 0],
                [1, 1, 1],
                [0, 0, 0],
            ],
        ],
        #  ##
        #   ##
        [
            [
                [0, 1, 0],
                [1, 1, 0],
                [1, 0, 0],
            ],
            [
                [0, 0, 0],
                [1, 1, 0],
                [0, 1, 1],
            ],
        ],
        #   ##
        #  ##
        [
            [
                [0, 1, 0],
                [0, 1, 1],
                [0, 0, 1],
            ],
            [
                [0, 0, 0],
                [0, 1, 1],
                [1, 1, 0],
            ],
        ],
    ]

    fieldx1 = 32
    fieldy1 = 10
    scorex1 = 11
    scorey1 = 11

    class RectRedraw:
        x1 = None
        y1 = None
        x2 = None
        y2 = None

        def max(r, val, valmax):
            if val > valmax:
                return valmax
            return val

        def min(r, val, valmin):
            if val < valmin:
                return valmin
            return val

        def merge(r, x1, y1, x2, y2):
            if r.x1 is None or r.x1 > x1:
                r.x1 = r.min(x1, 0)
            if r.y1 is None or r.y1 > y1:
                r.y1 = r.min(y1, 0)
            if r.x2 is None or r.x2 < x2:
                r.x2 = r.max(x2, field_width)
            if r.y2 is None or r.y2 < y2:
                r.y2 = r.max(y2, field_height)
            # print r.x1,r.y1,r.x2,r.y2

        def clean(r):
            r.x1 = None
            r.y1 = None
            r.x2 = None
            r.y2 = None
    rr = RectRedraw()
    for _ in range(field_height):
        field.append([0] * field_width)

    def echo(s):
        global charcache
        charcache += s
    #assert term.height > (field_height + 1)
    #echo_unbuffered(u''.join((
    #    u'\r\n\r\n',
    #    u'REAdY YOUR tERMiNAl %s ' % (term.bold_blue('(!)'),),
    #    u'\r\n\r\n',
    #    u'%s PRESS ANY kEY' % (term.bold_black('...'),),
    #)))
       
    
    #getch()
    # set syncterm font to cp437
    #if term.kind.startswith('ansi'):
    #    echo_unbuffered(syncterm_setfont('cp437'))
    #artfile = os.path.join(os.path.dirname(__file__), 'art', 'tetris.ans')
    #echo_unbuffered(u'\r\n' * term.height)  # cls
    #if os.path.exists(artfile):
    #    echo_unbuffered(from_cp437(open(artfile).read()).rstrip())
    
    # Turn off the Cursor
    bbs_io.pyPipe2Ansi('\x1b[?25l') 
    
    # Display Welcome Message
    bbs_io.pyPipe2Ansi('|CS|CR|CR|09REAdY YOUR tERMiNAl! |CR|CR|03PRESS ANY kEY')

    # get key from user, wait for input.
    bbs_io.pyGetKey()
    
    # Clear the Screen First
    bbs_io.pyPipe2Ansi('|CS')
    
    # Display ANSI screen .ans extension is assumed.
    bbs_io.pyAnsiPrintf('tetris')

    def gotoxy(x, y):
        #echo(term.move(y, x))
        echo('|XY%02d%02d' % (x+1, y+1))

    def plotblock(color, lastcolor):
        if color:
            #c = u'\u2588\u2588'  # '\xDB\xDB'
            c = chr(219) + chr(219)
        else:  # both empty
            c = '  '
            color = 0
        # Output optimization
        if color % 8 == 0:
            color = color / 8
        if color == lastcolor:
            echo(c)
        else:
            if color:
                fg = str(30 + color % 8)
            else:
                fg = '37'
            if color >= 8:
                bg = ';%d' % (40 + color / 8)
            else:
                bg = ''
            echo('\x1b[0;' + fg + bg + 'm')
            echo(c)
            lastcolor = color
        return lastcolor

    def drawfield():
        lastcolor = ''
        for y in range(0, field_height, 2):
            gotoxy(fieldx1 + 2, fieldy1 + 1 + y / 2)
            # Which block to show, full, half-up, half-down or empty.
            for x in range(field_width):
                color = field[y][x] + field[y + 1][x] * 8
                if field[y][x] and field[y + 1][x]:
                    #c = u'\u2588'  # '\xDB'
                    #c = '_'
                    c = chr(219)
                    if field[y][x] == field[y + 1][x]:
                        color = color % 8
                    else:
                        #c = u'\u2580'  # '\xDF'
                        #c = '-'
                        c = chr(223)
                elif field[y][x] and not field[y + 1][x]:
                    #c = u'\u2580'  # '\xDF'
                    #c = '+'
                    c = chr(223)
                elif not field[y][x] and field[y + 1][x]:
                    #c = u'\u2584'  # '\xDC'
                    #c = '-'
                    c = chr(220)
                else:  # both empty
                    c = ' '
                # Output optimization
                if color % 8 == 0:
                    color = color / 8
                if color == lastcolor:
                    echo(c)
                else:
                    if color:
                        fg = str(30 + color % 8)
                    else:
                        fg = '37'
                    if color >= 8:
                        bg = ';%d' % (40 + color / 8)
                    else:
                        bg = ''
                    echo('\x1b[0;' + fg + bg + 'm')
                    echo(c)
                    lastcolor = color
        #echo(term.normal)
        echo('|07|16') # reset background color

    layoutcolor = [7, 2, 7, 6, 3, 6, 3]
    # p    = -1  # Current piece type
    nextpiece = randint(0, len(layout) - 1)
    p = randint(0, len(layout) - 1)
    p = 1
    r = 0   # Current rotation
    xpos = 4   # X position
    # ypos = -2  # Y position
    ypos = -len(layout[p][0])
    level = 1
    score = 0
    lines = 0

    def flush():
        global charcache
        #echo_unbuffered(charcache)
        bbs_io.pyPipe2Ansi(charcache)
        charcache = ''

    def fillpiece(x, y, p, r, value):
        row = 0
        for line in layout[p][r]:
            col = 0
            for c in line:
                if c and (y + row) >= 0:
                    field[y + row][x + col] = value
                col += 1
            row += 1

    def showpiece(x, y, p, r):
        fillpiece(x, y, p, r, layoutcolor[p])

    def hidepiece():
        fillpiece(xpos, ypos, p, r, 0)

    def testpiece(x, y, newr):
        hidepiece()
        # Space at the new location?
        row = 0
        for line in layout[p][newr]:
            col = 0
            for c in line:
                try:
                    if c:
                        if ((y + row) >= 0 and field[y + row][x + col]
                                or (x + col) < 0 or (x + col) > 9):
                            return 0
                except IndexError:
                    return 0
                col += 1
            row += 1
        # Movement possible
        return 1

    def movepiece(x, y, newr):
        if testpiece(x, y, newr):
            # Build redraw rectangle
            rr.merge(xpos, ypos,
                     xpos + len(layout[p][0][0]), ypos + len(layout[p][0]))
            rr.merge(x, y,
                     x + len(layout[p][0][0]), y + len(layout[p][0]))
            showpiece(x, y, p, newr)
            return (x, y, newr, 1)
        else:
            showpiece(xpos, ypos, p, r)
            return (xpos, ypos, r, 0)

    def shownext(p):
        r = 0
        for y in range(4):
            gotoxy(26, 18 + y)
            echo(' ' * 4)

        #echo(term.color(layoutcolor[p]))
        #echo('|04')
        bbs_io.pyPipe2Ansi('|%02d' % (layoutcolor[p]))

        yoffset = int(len(layout[p][r][0]) < 4)
        xoffset = int(len(layout[p][r]) < 3)
        for y in range(len(layout[p][r])):
            for x in range(len(layout[p][r][y])):
                val = layout[p][r][y][x]
                if val:
                    gotoxy(26 + x + xoffset, 18 + y + yoffset)
                    #echo(u'\u2588\u2588')
                    #echo('**')
                    echo(chr(219))
                    echo(chr(219))

    def drawstats():
        #echo(term.move(scorey1, scorex1) + '%d' % level)
        #echo(term.move(scorey1 + 2, scorex1) + '%d' % lines)
        #echo(term.move(scorey1 + 3, scorex1) + '%d' % score)
        bbs_io.pyPipe2Ansi('|XY%02d%02d%d' % (scorex1, scorey1+1, level))
        bbs_io.pyPipe2Ansi('|XY%02d%02d%d' % (scorex1, scorey1+3, lines))
        bbs_io.pyPipe2Ansi('|XY%02d%02d%d' % (scorex1, scorey1+4, score))

    drawstats()
    ticksize = 0.4
    nexttick = time.time() + ticksize
    showpiece(xpos, ypos, p, r)
    gotoxy(26, 17)
    #echo(term.blue_reverse('next'))
    echo('|09|17n|00|17ext|07|16')
    shownext(nextpiece)

    # Full redraw first frame
    rr.merge(0, 0, field_width, field_height)

    buf = ''
    key = ''
    while True:
        drawfield()
        # gotoxy(0,0)
        # echo('\x1b[37mx: %d, y: %d, p: %d         '%(xpos,ypos,p))
        slice = nexttick - time.time()
        if slice < 0:
            slice = 0
        echo(buf)
        buf = ''
        flush()
        #key = getch(slice + 0.01)
        
        if stdin in select([stdin], [], [], slice + 0.01)[0]:   
            key = bbs_io.pyGetKey()
            if key != '':
                # if we got an escape sequence, then reassign the key
                if key == '27':
                    # Get Extended Sequence
                    key = bbs_io.pyGetKeyExtended()
                else:
                    #translate int to char value.
                    key = chr(int(key))
        else:        
            key = ''
        
        now = time.time()
        # hidepiece()
        if key is not None:
            if key in ('q', 'Q'):
                return (0, 0, 0)
            elif key in ('[D', 'h',):
                xpos, ypos, r, m = movepiece(xpos - 1, ypos, r)
            elif key in ('[C', 'l',):
                xpos, ypos, r, m = movepiece(xpos + 1, ypos, r)
            elif key in ('[A', 'k',):
                xpos, ypos, r, m = movepiece(
                    xpos, ypos, (r + 1) % len(layout[p]))
            elif key in ('[B', 'j',):
                xpos, ypos, r, m = movepiece(xpos, ypos + 1, r)
            elif key in (' ',):
                m = True
                c = 0
                while m:
                    xpos, ypos, r, m = movepiece(xpos, ypos + 1, r)
                    if m:
                        c += 1
                if c:
                    nexttick = time.time() + ticksize
        
        # New tick?
        if now > nexttick:
            nexttick += ticksize
            # Move down piece
            xpos, ypos, r, moved = movepiece(xpos, ypos + 1, r)
            # Piece has touched down?
            if not moved:
                # Is the player dead?
                if ypos <= -len(layout[p][0]):
         
                    # Full Redraw       
                    bbs_io.pyPipe2Ansi('|XY3324|12!! |04gAME OVeR |12!!|07')
                    
                    bbs_io.pyGetKey()
                     
                
#                    death_win = AnsiWindow(height=6, width=40,
#                                           yloc=fieldy1 + 10 / 2, xloc=fieldx1 - 11)
#                    death_win.colors['border'] = term.bold_black
#                    echo_unbuffered(death_win.clear() + death_win.border())
#                    echo_unbuffered(
#                        term.move(fieldy1 + 10 / 2 + 1, fieldx1 - 11))
#                    echo_unbuffered((
#                                    u'!! gAME OVeR!! Score was: %i' % (score,)).center(40))
#                    echo_unbuffered(
#                        term.move(fieldy1 + 10 / 2 + 3, fieldx1 - 11))
#                    echo_unbuffered(u'press RETURN'.center(40))
#                    while True:
#                        inp = getch()
#                        if inp in (u'\r', term.KEY_ENTER):
#                            break
                    return (score, level, lines)

                # Any complete rows to remove?
                complete = []
                for y in range(field_height):
                    x = 0
                    while x < field_width:
                        if field[y][x] == 0:
                            break
                        x += 1
                    if x == field_width:
                        complete.append(y)
                if len(complete) > 0:
                    # Add score
                    lines += len(complete)
                    score += len(complete) * len(complete) * 100
                    # Shrink field
                    for line in complete:
                        del field[line]
                        field.insert(0, [0] * field_width)

                    if lines >= level * 10:
                        level += 1
                        ticksize = 0.4 - level * 0.02
                    drawstats()

                    # Redraw complete field
                    rr.merge(0, 0, field_width, field_height)

                # Time for a new piece
                p = nextpiece
                nextpiece = randint(0, len(layout) - 1)
                r = 0
                xpos = 4
                ypos = -len(layout[p][0])
                showpiece(xpos, ypos, p, r)
                shownext(nextpiece)

# Start Main Loop
main()

# Turn cursor back on
bbs_io.pyPipe2Ansi('\x1b[?25h')
bbs_io.pyPipe2Ansi('|07|16')  

# Garbage Collection to free memory
gc.collect()




