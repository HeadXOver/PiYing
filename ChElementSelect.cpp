#include "ChElementSelect.h"
#include "KeyboardStateWin.h"
#include "piYingGL.h"

void ChElementSelect::deleteElement()
{
    std::vector<unsigned int>& idx = glIndex;
    std::vector <float>& vert = glVert;
    QList<QPointF>& s_Vert = sVert;
    const size_t nVert = s_Vert.size();
    const size_t nTri = idx.size() / 3;

    std::vector<bool> killVert(nVert, false);
    std::vector<bool> killTri(nTri, false);
    for (unsigned v : index) killVert[v] = true;

    std::vector<unsigned> refCount(nVert, 0);
    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killVert[v0] || killVert[v1] || killVert[v2]) {
            killTri[t] = true;
        }
        else {
            refCount[v0]++;
            refCount[v1]++;
            refCount[v2]++;
        }
    }

    for (size_t t = 0; t < nTri; ++t) {
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        if (killTri[t] == true) {
            if (!killVert[v1] && refCount[v1] == 0) killVert[v1] = true;
            if (!killVert[v2] && refCount[v2] == 0) killVert[v2] = true;
            if (!killVert[v0] && refCount[v0] == 0) killVert[v0] = true;
        }
    }

    std::vector<unsigned> old2new(nVert, UINT_MAX);
    unsigned newVertCount = 0;
    for (unsigned old = 0; old < nVert; ++old) {
        if (!killVert[old]) {
            old2new[old] = newVertCount++;
            vert[newVertCount + newVertCount - 2] = vert[old + old];
            vert[newVertCount + newVertCount - 1] = vert[old + old + 1];
            s_Vert[newVertCount - 1] = s_Vert[old];
        }
    }
    if (newVertCount == 0) {
        vert.clear();
        s_Vert.clear();
    }
    else {
        vert.resize(newVertCount * 2);
        s_Vert.resize(newVertCount);
    }

    size_t outIdx = 0;
    for (size_t t = 0; t < nTri; ++t) {
        if (killTri[t]) continue;
        unsigned v0 = idx[t * 3], v1 = idx[t * 3 + 1], v2 = idx[t * 3 + 2];
        idx[outIdx++] = old2new[v0];
        idx[outIdx++] = old2new[v1];
        idx[outIdx++] = old2new[v2];
    }
    if (outIdx != 0) idx.resize(outIdx);
    else idx.clear();

    index.clear();
}

void ChElementSelect::drawHandle(QPainter& painter)
{
    if(index.size() == 0) return;

    painter.setBrush(QColor(0, 0, 0, 0));

    // 计算中心点
    handleCenterPoint = QPointF();
    for (unsigned int i : index) handleCenterPoint += sVert[i];
    
    handleCenterPoint = gl->mapViewProjMatrix(handleCenterPoint / index.size());

	// 绘制圆
    painter.setPen(QPen(Qt::black, 4));
	painter.drawEllipse(handleCenterPoint, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);
    painter.setPen(QPen(Qt::yellow, 2));
    painter.drawEllipse(handleCenterPoint, ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS);

    // 绘制移动控制柄
	painter.setPen(QPen(Qt::black, 6));
    painter.drawLine(handleCenterPoint.x() - MOVEHANDLE_LENTH, handleCenterPoint.y(), handleCenterPoint.x() + MOVEHANDLE_LENTH, handleCenterPoint.y());
    painter.drawLine(handleCenterPoint.x(), handleCenterPoint.y() - MOVEHANDLE_LENTH, handleCenterPoint.x(), handleCenterPoint.y() + MOVEHANDLE_LENTH);
    painter.setPen(QPen(Qt::green, 4));
    painter.drawLine(handleCenterPoint.x() - MOVEHANDLE_LENTH, handleCenterPoint.y(), handleCenterPoint.x() + MOVEHANDLE_LENTH, handleCenterPoint.y());
    painter.setPen(QPen(Qt::red, 4));
    painter.drawLine(handleCenterPoint.x(), handleCenterPoint.y() - MOVEHANDLE_LENTH, handleCenterPoint.x(), handleCenterPoint.y() + MOVEHANDLE_LENTH);

    // 绘制中心点
    painter.setPen(QPen(Qt::black, 12));
    painter.drawPoint(handleCenterPoint);
    painter.setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter.drawPoint(handleCenterPoint);

	// 绘制缩放控制柄
    painter.setPen(QPen(Qt::black, 12));
    painter.drawPoint(handleCenterPoint.x(), handleCenterPoint.y() + SCALEHANDLE_DISTANCE);
    painter.drawPoint(handleCenterPoint.x() + SCALEHANDLE_DISTANCE, handleCenterPoint.y());
    painter.drawPoint(handleCenterPoint.x() + ROTATEHANDLE_RADIUS, handleCenterPoint.y() + ROTATEHANDLE_RADIUS);
    painter.setPen(QPen(Qt::yellow, HANDLE_ZONE));
    painter.drawPoint(handleCenterPoint.x(), handleCenterPoint.y() + SCALEHANDLE_DISTANCE);
    painter.drawPoint(handleCenterPoint.x() + SCALEHANDLE_DISTANCE, handleCenterPoint.y());
    painter.drawPoint(handleCenterPoint.x() + ROTATEHANDLE_RADIUS, handleCenterPoint.y() + ROTATEHANDLE_RADIUS);

}

void ChElementSelect::changeEditMode()
{
    if (index.size() == 0) {
        editMode = ChElementEditMode::None;
        return;
    }

    QPointF mouse = gl->mapViewProjMatrix(lastPos);

	// 判断是否在中心点上
    if (isInRect(mouse, handleCenterPoint, HANDLE_ZONE)) {
        editMode = ChElementEditMode::Move;
        QMessageBox::information(gl, "提示", "移动");
        return;
	}

    // 判断是否在移动控制柄X上
    if (isInRect(mouse, handleCenterPoint, MOVEHANDLE_LENTH * 2, HANDLE_ZONE)) {
        editMode = ChElementEditMode::MoveX;
        QMessageBox::information(gl, "提示", "移动X");
        return;
	}

    // 判断是否在移动控制柄Y上
    if (isInRect(mouse, handleCenterPoint, HANDLE_ZONE, MOVEHANDLE_LENTH * 2)) {
        editMode = ChElementEditMode::MoveX;
        QMessageBox::information(gl, "提示", "移动Y");
        return;
    }

    // 判断是否在旋转控制柄上
    QLineF line(handleCenterPoint, mouse);
	qreal length = line.length();
    if (length <= ROTATEHANDLE_RADIUS + HANDLE_ZONE && length >= ROTATEHANDLE_RADIUS - HANDLE_ZONE) {
        editMode = ChElementEditMode::Rotate;
		QMessageBox::information(gl, "提示", "旋转");
        return;
	}

    // 判断是否在缩放控制柄上
    if (isInRect(mouse, handleCenterPoint + QPoint(ROTATEHANDLE_RADIUS, ROTATEHANDLE_RADIUS), HANDLE_ZONE)) {
        editMode = ChElementEditMode::Scale;
        QMessageBox::information(gl, "提示", "缩放");
        return;
    }

	// 判断是否在缩放控制柄X上
    if (isInRect(mouse, handleCenterPoint + QPoint(SCALEHANDLE_DISTANCE, 0), HANDLE_ZONE)) {
        editMode = ChElementEditMode::ScaleX;
        QMessageBox::information(gl, "提示", "缩放X");
        return;
	}

	// 判断是否在缩放控制柄Y上
    if (isInRect(mouse, handleCenterPoint + QPoint(0, SCALEHANDLE_DISTANCE), HANDLE_ZONE)) {
        editMode = ChElementEditMode::ScaleY;
        QMessageBox::information(gl, "提示", "缩放Y");
        return;
	}

	editMode = ChElementEditMode::None;
}
