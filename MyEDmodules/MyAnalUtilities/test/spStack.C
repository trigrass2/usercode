#include "THStack.h"

struct wStack_t {
  wStack_t() : stack(NULL) {}
  THStack *stack;
  std::vector<wTH1 *> v_histos; /* have to keep the histos around for
				   legend labels and drawoptions */
};

static map<string, wStack_t *>      glmap_id2stack;

//======================================================================

//======================================================================

bool                              // returns true if success
processStackSection(FILE *fp,
		    string& theline,
		    bool& new_section)
{
  vector<string> v_tokens;

  wTH1  *stkwh = NULL; /* will become the histogram used to draw the stack,
			  collect user preferences here. */
  string  *sid = NULL;
  wStack_t *ws = NULL;

  cout << "Processing stack section" << endl;

  new_section=false;

  while (getLine(fp,theline,"stack")) {
    if (!theline.size()) continue;
    if (theline[0] == '#') continue; // comments are welcome

    if (theline[0] == '[') {
      new_section=true;
      break;
    }

    string key, value;
    if (!getKeyValue(theline,key,value)) continue;

    //--------------------
    if (key == "id") {
    //--------------------
      if (sid != NULL) {
	cerr << "no more than one id per stack section allowed " << value << endl;
	break;
      }

      sid = new string(value);

    //------------------------------
    } else if (key == "histos2stack") {
    //------------------------------
      if (!sid) {
	cerr << "id key must be defined first in the section" << endl; continue;
      }

      Tokenize(value,v_tokens," ,");
      if (!v_tokens.size()) v_tokens.push_back(value);

      if (ws) {
	cerr << "histo already defined" << endl; continue;
      }

      for (unsigned j = 0; j < v_tokens.size(); j++) {
	string& hid = v_tokens[j];
	map<string,wTH1 *>::const_iterator it = glmap_id2histo.find(hid);
	if (it == glmap_id2histo.end()) {
	  cerr << "ERROR: histo id " << hid << " never defined in layout" << endl;
	  exit (-1);
	}

	wTH1 *wh = it->second;
	if (!wh) {
	  cerr<< "spStack: find returned NULL wTH1 pointer for " << hid << endl;
	  continue;
	}

	if (!ws) {
	  ws        = new wStack_t();
	  ws->stack = new THStack(wh->histo()->GetName(),wh->histo()->GetTitle());
	  stkwh     = wh->Clone(wh->histo()->GetName(),wh->histo()->GetTitle());
	  stkwh->histo()->Reset();
	}

	ws->stack->Add(wh->histo());
	ws->v_histos.push_back(wh);
      } // token loop

    } else if (!ws) {  // all other keys must have one of the above ops defined 1st
      cerr << "a histos2stack key must be defined before key " << key << endl;
      break;
    }

    else {
      processCommonHistoParams(key,value,*stkwh);
      if (key=="title") ws->stack->SetTitle(stkwh->histo()->GetTitle());
    }
  } // while getline loop

  if  (ws) {
    // Done defining, now we can set the histogram of the stack
    ws->stack->SetHistogram(stkwh->histo()); // only available in 5.26+

    // How to control the y-axis of a stack simply?
    ws->stack->SetMaximum(stkwh->histo()->GetYaxis()->GetXmax());
    ws->stack->SetMinimum(stkwh->histo()->GetYaxis()->GetXmin());

    glmap_id2stack.insert(pair<string,wStack_t *>(*sid,ws));

    // Don't need this anymore - happily the destructor leaves the TH1 intact
    delete stkwh;
  }
  return (ws != NULL);
}                                                 // processStackSection

//======================================================================