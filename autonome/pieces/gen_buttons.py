#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-x","--width",dest="width",type="int",help="final width",default=210)
parser.add_option("-y","--height",dest="height",type="int",help="final height",default=297)
parser.add_option("-m","--margin",dest="margin",type="int",help="final margin",default=2)
parser.add_option("-f","--frame-border",dest="border",type="int",help="frame border",default=12)
parser.add_option("-s","--button-size",dest="button_size",type="int",help="button size",default=15)
parser.add_option("-b","--button-border",dest="button_border",type="int",help="button border",default=10)

(options, args) = parser.parse_args()

button_border = options.button_border
button_size = options.button_size

border = options.border
margin = options.margin
width = options.width
height = options.height

def header(content):
    s_header = r'<svg width="%.0fmm" height="%.0fmm" viewBox="0 0 %f %f"><g style="stroke:black; fill:none; stroke-width:.3;" transform="translate(%f,%f)">' % (width,height,width,height,margin,margin)
    e_header = r'</g></svg>'
    return '\n'.join((s_header,content,e_header))

def button(ii,jj):
    x = button_border/2 + (button_border+button_size) * ii
    y = button_border/2 + (button_border+button_size) * jj
    templ = r'<rect x="%f" y="%f" width="%f" height="%f" rx="1.5" />' % (x,y,button_size,button_size)
    return templ

def buttons(ni=8,nj=8):
    templ = []
    for ii in xrange(ni):
        for jj in xrange(nj):
            templ.append( button(ii,jj) )
    templ.append( r'<rect x="0" y="0" width="%f" height="%f" style="stroke-dasharray:2.5,2.5;" />' % ( ni*(button_size+button_border),nj*(button_size+button_border) ) )
    return "\n".join( templ )

def frame(content,ni=8,nj=8):
    tot = r'<rect x="0" y="0" width="%f" height="%f" />' % ( 2*border + ni*(button_size+button_border) - button_border,2*border + nj*(button_size+button_border) - button_border )
    s_border = r'<g transform="translate(%f,%f)">' % ( border - button_border/2,border - button_border/2 )
    e_border = r'</g>'
    return '\n'.join((tot,s_border,content,e_border))

svg = header( frame( buttons() ) )
print svg

