import pkg_resources

import _webrtcaec

__author__ = "Chang Sun"
__copyright__ = "Copyright (C) 2021 Chang Sun"
__license__ = ""
__version__ = pkg_resources.get_distribution('webrtcaec').version


class aec(object):
    def __init__(self, sample_rate=16000):
        self._aec = _webrtcaec.create()
        _webrtcaec.init(self._aec, sample_rate)

    def process(self, far, near, msInSndCardBuf=0, skew=0):
        return _webrtcaec.process(self._aec, msInSndCardBuf, skew, far, near)
