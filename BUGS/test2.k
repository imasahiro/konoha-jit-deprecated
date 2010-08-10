class EvidenceLog;
class AnalyzerStatus;
class SampleLog extends EvidenceLog;
class SampleStatus extends AnalyzerStatus;

class EvidenceStream {

       String name;

       EvidenceStream() {};

       void add (String aname) {

       }

       void start (String aname, AnalyzerStatus status) {

       }

       void stop (String aname) {

       }

       void close(String aname) {

       }

       void analyze (SampleLog log, SampleStatus status) {

       }
}

