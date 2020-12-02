#include <cpgplot.h>
#include <string.h>
#include "ast.h"
#include "fitsio.h"
int plotGrid(AstFrameSet* wcsinfo, double imageX1, double imageX2, double imageY1, double imageY2, double width, double height,
    double paddingLeft, double paddingRight, double paddingTop, double paddingBottom, const char* args) {
    if (!wcsinfo) {
        return 1;
    }
    AstPlot* plot;
    double hi = 1, lo = -1, scale, x1 = paddingLeft, x2 = width - paddingRight, xleft, xright, xscale;
    double y1 = paddingBottom, y2 = height - paddingTop, ybottom, yscale, ytop;
    double nx = imageX2 - imageX1;
    double ny = imageY2 - imageY1;
    xscale = (x2 - x1) / nx;
    yscale = (y2 - y1) / ny;
    scale = (xscale < yscale) ? xscale : yscale;
    xleft = 0.5f * (x1 + x2 - nx * scale);
    xright = 0.5f * (x1 + x2 + nx * scale);
    ybottom = 0.5f * (y1 + y2 - ny * scale);
    ytop = 0.5f * (y1 + y2 + ny * scale);
    printf("xleft = %f\n", xleft);
    printf("xright = %f\n", xright);
    printf("ybottom = %f\n", ybottom);
    printf("ytop = %f\n", ytop);
    printf("imageX1 = %f\n", imageX1);
    printf("imageY1 = %f\n", imageY1);
    printf("imageX2 = %f\n", imageX2);
    printf("imageY2 = %f\n", imageY2);
    float gbox[] = {(float)xleft, (float)ybottom, (float)xright, (float)ytop};
    double pbox[] = {imageX1, imageY1, imageX2, imageY2};
    plot = astPlot(wcsinfo, gbox, pbox, args);
    astBBuf(plot);
    astGrid(plot);
    astEBuf(plot);
    astAnnul(plot);
    if (!astOK) {
        astClearStatus;
        return 1;
    }
    return 0;
}
int main() {
    fitsfile* fptr;
    int status;
    char filename[] = "aJ.fits"; /* name of existing FITS file   */
    if (fits_open_file(&fptr, filename, READONLY, &status)) {
        printf("Can't open the file!\n");
    }
    char* header;
    int nkeys, status2;
    AstFitsChan* fitschan;
    AstFrameSet* wcsinfo;
    if (!fits_hdr2str(fptr, 0, NULL, 0, &header, &nkeys, &status2)) {
        // printf("header: %s", header);
        // printf("nkeys: %d", nkeys);
        // printf("status2: %d", status2);
        fitschan = astFitsChan(NULL, NULL, "");
        astPutCards(fitschan, header);
        // printf("header: %s", header);
        // free(header);
        wcsinfo = astRead(fitschan);
        astShow(wcsinfo);
    }
    float hi, lo, scale, x1, x2, xleft, xright, xscale;
    float y1, y2, ybottom, yscale, ytop;
    int nx, ny;
    char text[80];
    strcpy(text, "/xwindow");
    if (cpgbeg(0, text, 1, 1) == 1) {
        cpgpage();
        cpgwnad(0.0f, 512.0f, 0.0f, 1024.0f);
        cpgqwin(&x1, &x2, &y1, &y2);
        printf("x1 = %f\n", x1);
        printf("x2 = %f\n", x2);
        printf("y1 = %f\n", y1);
        printf("y2 = %f\n", y2);
        // xscale = (x2 - x1) / nx;
        // yscale = (y2 - y1) / ny;
        // scale = (xscale < yscale) ? xscale : yscale;
        // xleft = 0.5f * (x1 + x2 - nx * scale);
        // xright = 0.5f * (x1 + x2 + nx * scale);
        // ybottom = 0.5f * (y1 + y2 - ny * scale);
        // ytop = 0.5f * (y1 + y2 + ny * scale);
        //{
        //    float tr[] = {xleft - 0.5f * scale, scale, 0.0f, ybottom - 0.5f * scale, 0.0f, scale};
        //    cpggray(data, nx, ny, 1, nx, 1, ny, hi, lo, tr);
        //}
        int plot_ststus = plotGrid(wcsinfo, 0, 512, 0, 1024, 512, 1024, 0, 0, 0, 0, "Grid=1, Colour(grid)=2, Font(textlab)=3");
        printf("plot_ststus = %d\n", plot_ststus);
        cpgend();
    }
    return 0;
}