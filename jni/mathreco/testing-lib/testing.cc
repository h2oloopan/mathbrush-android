#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

#include "MathRecognizer.h"
#include "annotate.h"
#include "mathrecognizer-private.h"
#include "testing.h"
#include "timing.h"


namespace scg {


const std::string TEST_RESULTS_TABLE_NAME("test_results");

/*enum status {
	ST_NONE,
	ST_RETRY,
	ST_MATCH
};*/
typedef bool status;

test_context::test_context()
	: db(0), flags(TEST_DEFAULT_MODE)
{ }


const char *
get_stat_column_name(size_t i) {
	if (i > STAT__N) {
		THROW_ERROR(E_INVALID, "There are only " << STAT__N << " statistics, but you specified index " << i);
	}

	switch (i) {
	case STAT_TESTED:
		return "tested";
	case STAT_FEASIBLE:
		return "feasible";
	case STAT_RECOGNIZABLE:
		return "recognizable";
	case STAT_ATTAINABLE:
		return "attainable";
	case STAT_CORRECT:
		return "correct";
	case STAT_ABORTED:
		return "aborted";
	case STAT_CORRECT_GROUPS:
		return "correct_groups";
	case STAT_CORRECT_SYMBOLS:
		return "correct_symbols";
	case STAT_STRUCT_CORRECTIONS:
		return "struct_corr";
	case STAT_TERM_CORRECTIONS:
		return "term_corr";
	case STAT_VIEWS:
		return "views";
	case STAT_PARSE_TIME:
		return "parse_time";
	case STAT_EXTRACT_TIME:
		return "extract_time";
	case STAT_SEARCH_TIME:
		return "seek_time";
	case STAT_EXPR_LEN:
		return "nsymbols";
	case STAT_EXPR_HEIGHT:
		return "depth";
	case STAT_EXPR_RELS:
		return "nrels";
	case STAT_NSEGMENTS:
		return "nsegs";
	case STAT_NSPANS:
		return "nspans";
	case STAT_NGROUPS:
		return "ngroups";
	case STAT_NCELLS:
		return "ncells";
	case STAT_NLINKS:
		return "nlinks";
	default:
		THROW_ERROR(E_NOTFOUND, "Unspecified column name for statistic with index " << i);
	}
}
const char *
get_stat_name(size_t i) {
	if (i > STAT__N) {
		THROW_ERROR(E_INVALID, "There are only " << STAT__N << " statistics, but you specified index " << i);
	}

	switch (i) {
	case STAT_TESTED:
		return "Tested";
	case STAT_FEASIBLE:
		return "Feasible";
	case STAT_RECOGNIZABLE:
		return "Recognizable";
	case STAT_ATTAINABLE:
		return "Attainable";
	case STAT_CORRECT:
		return "Correct";
	case STAT_ABORTED:
		return "Aborted";
	case STAT_CORRECT_GROUPS:
		return "# Correct stroke groups";
	case STAT_CORRECT_SYMBOLS:
		return "# Correct symbols";
	case STAT_STRUCT_CORRECTIONS:
		return "Structural corrections";
	case STAT_TERM_CORRECTIONS:
		return "Terminal corrections";
	case STAT_VIEWS:
		return "Results viewed";
	case STAT_PARSE_TIME:
		return "Parse time";
	case STAT_EXTRACT_TIME:
		return "Extraction time";
	case STAT_SEARCH_TIME:
		return "Correction time";
	case STAT_EXPR_LEN:
		return "# Symbols";
	case STAT_EXPR_HEIGHT:
		return "Expression depth";
	case STAT_EXPR_RELS:
		return "# Relations";
	case STAT_NSEGMENTS:
		return "# Segments";
	case STAT_NSPANS:
		return "# Spans";
	case STAT_NGROUPS:
		return "# Groups";
	case STAT_NCELLS:
		return "# Parse cells";
	case STAT_NLINKS:
		return "# Parse links";
	default:
		THROW_ERROR(E_NOTFOUND, "Unspecified name for statistic with index " << i);
	}
}


const char *
get_stat_description(size_t i) {
	if (i > STAT__N) {
		THROW_ERROR(E_INVALID, "There are only " << STAT__N << " statistics, but you specified index " << i);
	}

	switch (i) {
	case STAT_TESTED:
		return "The file was tested.";
	case STAT_FEASIBLE:
		return "All symbols within the file appeared as recognition candidates.";
	case STAT_RECOGNIZABLE:
		return "The parser returned results for the file.";
	case STAT_ATTAINABLE:
		return "The correct expression was attainable after corrections.";
	case STAT_CORRECT:
		return "The first expression returned was correct.";
	case STAT_ABORTED:
		return "An unhandled error occured in the recognition library.";
	case STAT_CORRECT_GROUPS:
		return "correct_groups";
	case STAT_CORRECT_SYMBOLS:
		return "correct_symbols";
	case STAT_STRUCT_CORRECTIONS:
		return "Number of structural corrections required to obtain the correct expression.";
	case STAT_TERM_CORRECTIONS:
		return "Number of corrections to terminal symbols required to obtain the correct expression.";
	case STAT_VIEWS:
		return "Total number of candidate expressions viewed during the correction process.";
	case STAT_PARSE_TIME:
		return "Time required for the parser to analyze the input's structure.";
	case STAT_EXTRACT_TIME:
		return "Time required to extract the top-ranked parse tree from the parse graph.";
	case STAT_SEARCH_TIME:
		return "Time required to make corrections to the recognition results.";
	case STAT_EXPR_LEN:
		return "Number of symbols in the expression ground-truth.";
	case STAT_EXPR_HEIGHT:
		return "Depth of the ground-truth expression tree.";
	case STAT_EXPR_RELS:
		return "Number of geometrical relations in the expression ground-truth.";
	case STAT_NSEGMENTS:
		return "Number of strokes in the input expression.";
	case STAT_NSPANS:
		return "Number of stroke subsets considered during parsing.";
	case STAT_NGROUPS:
		return "Number of distinct stroke groups generated by the symbol recognizer.";
	case STAT_NCELLS:
		return "Number of nodes in the parse graph.";
	case STAT_NLINKS:
		return "Number of links in the parse graph.";
	default:
		THROW_ERROR(E_NOTFOUND, "Unspecified description for statistic with index " << i);
	}
}


struct test_result {
private:
	std::vector<double> measurements_;
	std::vector<bool> valid_;

public:
	test_result() {
		measurements_.resize(STAT__N);
		valid_.insert(valid_.end(), STAT__N, false);
	}

	double get(size_t i) const {
		if (!valid_[i]) {
			THROW_ERROR(E_NOTREADY, "Measurement " << i << " is not valid.\n");
		}
		return measurements_[i];
	}

	void set(size_t i, double x) {
		measurements_[i] = x;
		valid_[i] = true;
	}

	bool valid(size_t i) const { return valid_[i]; }
};



static Rect<long>
make_box(const RawStrokeGroup &G, const stroke_collection &strokes)
{
	stroke_collection::const_iterator i = strokes.begin();
	Rect<long> bounds = G.strokes[*i].bounds();//scg::bbox(G.strokes[*i]);
	for(++i; i != strokes.end(); ++i) {
		bounds = scg::merge(bounds, G.strokes[*i].bounds()/*scg::bbox(G.strokes[*i])*/);
	}
	return bounds;
}


struct local_stats {
	unsigned views;
	unsigned term_corrections;
	unsigned struct_corrections;

	status match_obtained;

	explicit local_stats(status mo)
		: views(0), term_corrections(0), struct_corrections(0),
		  match_obtained(mo)
		{ }
	local_stats(unsigned v, unsigned t, unsigned s)
		: views(v), term_corrections(t), struct_corrections(s),
		  match_obtained(true)//ST_MATCH)
		{ }
	
	local_stats &operator+=(const local_stats &rhs)
	{
		views += rhs.views;
		term_corrections += rhs.term_corrections;
		struct_corrections += rhs.struct_corrections;
		return *this;
	}
};


template <typename T, typename U>
static void
flatten(T first, T last, U &acc) {
	if (first == last) {
		if (acc == "...") {
			acc = "hdots";
		}
		return;
	}
	if (*first == "dot") {
		acc += ".";
	}
	else {
		acc += *first;
	}
	++first;
	flatten(first, last, acc);
}


local_stats
match_to_ground_truth(const portable_tree &spec, const ExpressionTree *node,
					  MathRecognizer *rec, ExpressionIterator *it, size_t limit) {
	if (limit == 0) {
		return local_stats(false);
	}

	if (node->type() == scg::PAREN_EXPR && spec.sid != scg::PAREN_EXPR) {// && parent_type == scg::FN_EXPR) {
		return match_to_ground_truth(spec, node->child(1), rec, it, limit-1);
	}

	if (spec.sid == scg::InvalidSemanticId
	 && node->type() == scg::TERMINAL_EXPR
	 && std::strlen(node->long_str()) == 0) {
		return local_stats(true);
	}


	if (spec.sid == node->type() && spec.children.size() == node->nchildren()) {
		if (spec.sid == scg::TERMINAL_EXPR) {
			//return local_stats(true);
			std::string content;
			flatten(spec.content.begin(), spec.content.end(), content);
			if (content == node->str()) {
				return local_stats(true);
			}
			else {
				if (!it) {
					return local_stats(false);
				}

				unsigned v = 1;
				for (node = it->next(); node; node = it->next()) {
					if (content == node->str()) {
						return local_stats(v, 1, 0);
					}
					++v;
				}
				return local_stats(false);
			}
		}
		else {
			local_stats stats(true);
			std::list<scg::portable_tree>::const_iterator ti = spec.children.begin();
			unsigned i;
			for (i = 0; i < node->nchildren(); ++i, ++ti) {
				const scg::ExpressionTree *child = node->child(i);
				scg::ExpressionIterator *child_it = rec->CreateSubtreeIterator(child);
				if (!child_it) {
					if (ti->sid == scg::InvalidSemanticId) {
						continue;
					}
					return local_stats(false);
				}
				child_it->next();
				local_stats child_stats = match_to_ground_truth(*ti, child, rec, child_it, limit-1);
				child_it->release();
				if (child_stats.match_obtained) {
					stats += child_stats;
				}
				else {
					break;
				}
			}

			if (i == node->nchildren()) {
				return stats;
			}
		}
	}

	if (!it) {
		return local_stats(false);
	}
	node = it->next();
	if (node) {
		local_stats stats = match_to_ground_truth(spec, node, rec, it, limit-1);
		++stats.views;
		if (stats.struct_corrections == 0) {
			++stats.struct_corrections;
		}
		return stats;
	}
	else {
		return local_stats(false);
	}
}

struct symbol_details {
	std::string name;
	size_t size;
};

static bool
check_sanity(const test_context *tctx, const scg::AnnotatedStrokeGroup &grp)
{
	std::vector<size_t> strokes;
	for (size_t k = 0; k < scg::num_strokes(grp); ++k) {
		strokes.push_back(k);
	}
	scg::AnnotatedStrokeGroup::const_symbol_iterator j;
	for (j = grp.symbols_begin(); j != grp.symbols_end(); ++j) {
		const scg::symbol_annotation &sa = *j;
		if (!(tctx->flags & TEST_SKIP_SYMBOL_RECOGNIZER)) {
			scg::symbol *S = scg::symdb_findsymbol_name(sa.name);
			if (!S) return false;
			scg::prototype *prot;
			for (prot = S->firstproto(); prot; prot = S->nextproto(prot)) {
				if (prot->strokes.nstrokes == sa.strokes.size()) break;
			}
			if (!prot) return false;
		}
		for (scg::stroke_collection::const_iterator k = sa.strokes.begin(); k != sa.strokes.end(); ++k) {
			std::vector<size_t>::iterator q;
			q = std::find(strokes.begin(), strokes.end(), *k);
			if (q == strokes.end()) {
				return false;
			}
			else {
				strokes.erase(q);
			}
		}
	}
	return strokes.empty();
}

static std::list<symbol_details>
find_missing_symbols(const scg::math_recognizer_base *rec, const scg::AnnotatedStrokeGroup &G, unsigned &corgroups, unsigned &corsymbols) {
	corgroups = corsymbols = 0;
	std::vector<size_t> strokes;
	for (size_t k = 0; k < G.nstrokes; ++k) {
		strokes.push_back(k);
	}
	std::list<symbol_details> missing;
	size_t sn = 0;
	for (scg::AnnotatedStrokeGroup::const_symbol_iterator i = G.symbols_begin(); i != G.symbols_end(); ++i) {
		symbol_details S;
		scg::Rect<long> bounds;
		// first check for the strokes and segments matching up
		scg::bitvec bits(rec->segments.size(), false);
		scg::stroke_collection::const_iterator j;
		bool try_segments = true;
		for (scg::stroke_collection::const_iterator j = i->strokes.begin(); j != i->strokes.end(); ++j) {
			if (*j >= rec->segments.size()) {
				try_segments = false;
			}
			else {
				bits[*j] = true;
			}

			if (j == i->strokes.begin()) {
				bounds = rec->strokes[*j]->input.bounds();//scg::bbox(rec->strokes[*j]->input);
			}
			else {
				bounds = merge(bounds, rec->strokes[*j]->input.bounds()/*scg::bbox(rec->strokes[*j]->input)*/);
			}
		}
		
		const scg::group *grp = 0;

		if (try_segments) {
			std::map<scg::bitvec, scg::group *>::const_iterator pgrp = rec->groups->find(bits);
			if (pgrp != rec->groups->end() && pgrp->second->bounds == bounds) {
				grp = pgrp->second;
			}
		}

		if (!grp) {
			for (std::map<scg::bitvec, scg::group *>::const_iterator pgrp = rec->groups->begin(); pgrp != rec->groups->end(); ++pgrp) {
				if (pgrp->second->bounds == bounds) {
					grp = pgrp->second;
				}
			}
		}

		if (grp) {
			++corgroups;
			scg::recoscore top_score = 0;
			bool topisright = false;
			for (std::map<const nonterminal *, scg::match_score>::const_iterator k = grp->matches.begin(); k != grp->matches.end(); ++k) {
				const scg::match_score &match = k->second;
				if (match.S->name == i->name) {
					if (match.final_score.score < top_score.score) {
						++corsymbols;
					}
					else {
						top_score = match.final_score;
						topisright = true;
					}
					for (size_t q = 0; q < bits.size(); ++q) {
						if (bits.at(q)) {
							std::vector<size_t>::iterator qq = std::find(strokes.begin(), strokes.end(), q);
							std::cout << "removing stroke " << q << " by segments\n";
							if (qq == strokes.end()) {
								symbol_details S;
								S.name = "dummy";
								S.size = 1;
								missing.push_back(S);
								return missing;
							}
							strokes.erase(qq);
						}
					}
				}
				else if (match.final_score.score > top_score.score) {
					top_score = match.final_score;
					topisright = false;
				}
			}
		}
		else {
			S.name = i->name;
			S.size = i->strokes.size();
			missing.push_back(S);
			std::cerr << "missing symbol " << S.name << " at " << sn << std::endl;
		}
		++sn;
	}

	for (std::vector<size_t>::const_iterator q = strokes.begin(); q != strokes.end(); ++q) {
		symbol_details S;
		S.name = "dummy";
		S.size = 1;
		missing.push_back(S);
		std::cerr << "missing stroke " << *q << std::endl;
	}
	return missing;
}


int
enable_test_logging(const std::string &log_filename)
{
	return E_UNSUPPORTED;
}


int
disable_test_logging()
{
	return E_UNSUPPORTED;
}


static int
write_result_to_db(sqlite3 *db, const std::string &filename, const test_result &tr)
{
	static unsigned n = 0;
	std::cerr << "Recording results for " << filename << "making " << ++n << std::endl;
	std::stringstream cmd;
	cmd << "INSERT INTO " << TEST_RESULTS_TABLE_NAME << " (filename";
	for (size_t i = 0; i < STAT__N; ++i) {
		if (tr.valid(i)) {
			cmd << ", " << get_stat_column_name(i);
		}
	}
	cmd << ") VALUES (\'" << filename << '\'';

	for (size_t i = 0; i < STAT__N; ++i) {
		if (tr.valid(i)) {
			cmd << ", " << tr.get(i);
		}
	}
	cmd << ')';

	int e = sqlite3_exec(db, cmd.str().c_str(), 0, 0, 0);
	if (e != SQLITE_OK) {
		std::cerr << "stest: sqlite INSERT error: " << sqlite3_errmsg(db) << std::endl;
		std::cerr << "stest: (command was " << cmd.str() << ")\n";
		return -1;
	}

	return 0;
}


int
run_tests(test_context *tctx)
{
	RecognizerHandle rh = scg::InitializeRecognizer();
	if (!rh) {
		return E_NOTREADY;
	}

	for (std::vector<std::string>::const_iterator i = tctx->filenames.begin(); i != tctx->filenames.end(); ++i) {
		test_result tr;

		AnnotatedStrokeGroup G;
		math_recognizer_base *rec = (math_recognizer_base *)CreateMathRecognizer();

		size_t ncells = 0;
		size_t nlinks = 0;

		int e;

		std::cerr << "Testing " << *i << std::endl;
		std::ifstream in(i->c_str());
		if (in.bad() || !in.is_open()) {
			tr.set(STAT_TESTED, 0);
			goto record_test;
		}

		std::cout << "Testing " << i->c_str() << "...";

		e = import_annotated_ink(in, G, GetMathGrammar(rh));
		if (FAILURE(e)) {
			std::cout << "invalid\n";
			tr.set(STAT_TESTED, 0);
			goto record_test;
		}

		if (check_sanity(tctx, G)) {
			tr.set(STAT_TESTED, 1);
		}
		else {
			goto record_test;
		}

		tr.set(STAT_TESTED, 1);

		/*scg::RawStroke *newstks = new scg::RawStroke[scg::num_strokes(G)];
		for (size_t i = 0; i < scg::num_strokes(G); ++i) {
			if (scg::num_points(G.strokes[i]) < 4) {
				long *newx = new long[1];
				long *newy = new long[1];
				newx[0] = G.strokes[i].x[0];
				newy[0] = G.strokes[i].y[0];
				newstks[i].set_points(newx, newy, 1);
			}
			else {
				long *newx = new long[scg::num_points(G.strokes[i]) / 4 + 1];
				long *newy = new long[scg::num_points(G.strokes[i]) / 4 + 1];
				for (size_t j = 0; j < scg::num_points(G.strokes[i]); j += 4) {
					newx[j/4] = G.strokes[i].x[j];
					newy[j/4] = G.strokes[i].y[j];
				}
				newstks[i].set_points(newx, newy, scg::num_points(G.strokes[i]) / 4 + 1);
			}
		}
		G.set_strokes(newstks, scg::num_strokes(G));*/

		if (tctx->flags & TEST_SKIP_SYMBOL_RECOGNIZER) {
			std::vector<MathSymbol> symbols;
			for (AnnotatedStrokeGroup::const_symbol_iterator i = G.symbols_begin(); i != G.symbols_end(); ++i) {
				Rect<long> box = make_box(G, i->strokes);
				const symbol *S = symdb_findsymbol_name(i->name);
				assert(S);
				symbols.push_back(MathSymbol(S->unicode, box));
			}

			try {
				rec->AddKnownSymbols(&symbols[0], symbols.size());
				tr.set(STAT_FEASIBLE, 1);
			}
			catch (...) {
				tr.set(STAT_ABORTED, 1);
				goto record_test;
			}
		}
		else {
			try {
				rec->AddStrokes(G);
			}
			catch (...) {
				tr.set(STAT_ABORTED, 1);
			}
			/*unsigned corgroups = 0, corsymbols = 0;
			std::list<symbol_details> missing_symbols = find_missing_symbols(rec, G, corgroups, corsymbols);
			if (!missing_symbols.empty()) {
				tr.set(STAT_FEASIBLE, 0);
				std::cout << "infeasible\n";
			}
			else {
				tr.set(STAT_FEASIBLE, 1);
			}
			tr.set(STAT_CORRECT_GROUPS, corgroups);
			tr.set(STAT_CORRECT_SYMBOLS, corsymbols);*/
		}

		tr.set(STAT_NSEGMENTS, num_strokes(G));
		tr.set(STAT_EXPR_LEN, G.symbol_annotations.size());
		tr.set(STAT_EXPR_RELS, G.link_annotations.size());
		tr.set(STAT_EXPR_HEIGHT, G.spec.height());
		tr.set(STAT_NSPANS, rec->spans->size());
		tr.set(STAT_NGROUPS, rec->groups->size());

		for (scg::parse_table::const_iterator j = rec->tab.begin(); j != rec->tab.end(); ++j) {
			ncells += j->second.size();
			/*for (scg::nt_parse_table::const_iterator k = j->second.begin(); k != j->second.end(); ++k) {
				if (k->second) {
					nlinks += k->second->links.size();
				}
			}*/
		}

		tr.set(STAT_NCELLS, ncells);
		//tr.set(STAT_NLINKS, nlinks);

		//if (tr.valid(STAT_FEASIBLE) && tr.get(STAT_FEASIBLE) != 0.0) {
		{
			ExpressionIterator *it;
			const ExpressionTree *tree = 0;
			try {
				TIME_OPERATION(
					it = rec->CreateDefaultIterator();
					if (it) tree = it->next();
				);
				tr.set(STAT_EXTRACT_TIME, ELAPSED);
			}
			catch (...) {
				std::cout << "aborted\n";
				tr.set(STAT_ABORTED, 1);
				goto record_test;
			}
			if (!tree) {
				std::cout << "unrecognizable\n";
				tr.set(STAT_RECOGNIZABLE, 0); 
				if (it) {
					it->release();
				}
			}
			else {
				tr.set(STAT_RECOGNIZABLE, 1);
				//if (tr.valid(STAT_FEASIBLE) && tr.get(STAT_FEASIBLE) != 0.0) {
					//it->release();
					local_stats stats(false);//ST_RETRY);
					try {
						TIME_OPERATION(
							stats = match_to_ground_truth(G.spec, tree, rec, it, 256);
							/*local_stats ls(ST_RETRY);
							do {
								it = rec->CreateDefaultIterator();
								ls = match(G.spec, it, rec);
								stats += ls;
								stats.match_obtained = ls.match_obtained;
								it->release();
							} while (ls.match_obtained == ST_RETRY);*/
						);
						tr.set(STAT_SEARCH_TIME, ELAPSED);
					}
					catch (...) {
						tree->release();
						it->release();
						tr.set(STAT_ABORTED, 1);
						goto record_test;
					}
					tree->release();
					it->release();

					if (stats.match_obtained == true) {//ST_MATCH) {
						tr.set(STAT_ATTAINABLE, 1);
						if (stats.views == 0) {
							std::cout << "correct\n";
							tr.set(STAT_CORRECT, 1);
						}
						else {
							std::cout << "attainable\n";
							tr.set(STAT_CORRECT, 0);
						}

						tr.set(STAT_TERM_CORRECTIONS, stats.term_corrections);
						tr.set(STAT_STRUCT_CORRECTIONS, stats.struct_corrections);
						tr.set(STAT_VIEWS, stats.views);
					}
					else {
						std::cout << "UNattainable\n";
						tr.set(STAT_ATTAINABLE, 0);
					}
				}
			//}
		}

		rec->release();

record_test:
		e = write_result_to_db(tctx->db, *i, tr);
		if (FAILURE(e)) {
			return e;
		}
	}

	return 0;
}


int
init_test_context(test_context *tctx)
{
	int e = sqlite3_open(tctx->dbname.c_str(), &tctx->db);
	if (e) {
		ERR(E_SYSTEM, "Failed opening sqlite db named " << tctx->dbname << ": " << sqlite3_errmsg(tctx->db));
		sqlite3_close(tctx->db);
		return ERROR_CODE();
	}

	std::stringstream cmd;
	cmd << "CREATE TABLE " << scg::TEST_RESULTS_TABLE_NAME << " (filename varchar(512)";
	for (size_t i = 0; i < scg::STAT__N; ++i) {
		cmd << ", " << scg::get_stat_column_name(i);
	}
	cmd << ')';

	e = sqlite3_exec(tctx->db, cmd.str().c_str(), 0, 0, 0);
	if (e != SQLITE_OK) {
		ERR(E_SYSTEM, "Failed creating sqlite db named " << tctx->dbname << ": " << sqlite3_errmsg(tctx->db));
		sqlite3_close(tctx->db);
		return ERROR_CODE();
	}

	return 0;
}


int
close_test_context(test_context *tctx)
{
	if (tctx->db) {
		sqlite3_close(tctx->db);
		tctx->db = 0;
	}
	return 0;
}

}
