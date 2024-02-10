#include "CatmullClark.hpp"



std::pair<double, double> calculateMiddlePoint(const std::pair<double, double>& a, const std::pair<double, double>& b) {
	double new_x = ((a.first + b.first) / 2.0f);
	double new_y = ((a.second + b.second) / 2.0f);

	return std::make_pair(new_x, new_y);
}

std::pair<double, double> makeSmooth(std::pair<double, double> current_point, std::pair<double, double> prev_mid_point, const std::pair<double, double> next_mid_point) {


	double new_x = (current_point.first * 2.0f + prev_mid_point.first + next_mid_point.first) * 0.25f;
	double new_y = (current_point.second * 2.0f + prev_mid_point.second + next_mid_point.second) * 0.25f;

	return std::make_pair(new_x, new_y);
}


std::pair<double, double> makeRough(std::pair<double, double> current_point, std::pair<double, double> prev_mid_point, const std::pair<double, double> next_mid_point) {

	double new_x = (4 * current_point.first - prev_mid_point.first - next_mid_point.first) / 2.0f;
	double new_y = (4 * current_point.second - prev_mid_point.second - next_mid_point.second) / 2.0f;
	return std::make_pair(new_x, new_y);
}


const void CatmullClark::subDivision(std::vector<std::pair<double, double>>& coords) {
	std::vector<std::pair<double, double>> coords_backup(coords);

	coords.clear();
	for (size_t i = 0; i < coords_backup.size(); ++i) {
		std::pair<double, double> current_point = coords_backup.at(i);
		if (i == 0 || i == coords_backup.size() - 1) {
			if (i == 0) {
				std::pair<double, double> next_point = coords_backup.at(i + 1);
				std::pair<double, double> next_mid_point = calculateMiddlePoint(current_point, next_point);
				coords.push_back(next_mid_point);
			}
			coords.push_back(current_point);
		}else  {
			std::pair<double, double> next_point = coords_backup.at(i+1);
			std::pair<double, double> prev_point = coords_backup.at(i-1);
			std::pair<double, double> next_mid_point = calculateMiddlePoint(current_point, next_point);
			std::pair<double, double> prev_mid_point = calculateMiddlePoint(current_point, prev_point);
			std::pair<double, double> smoother = makeSmooth(current_point, prev_mid_point, next_mid_point);
			coords.push_back(next_mid_point);
			coords.emplace_back(smoother);
			/*std::cout << "current: " << current_point.first << " : " << current_point.second << " | "
				<< "prev: " << prev_mid_point.first << " : " << prev_mid_point.second << " | "
				<< "next: " << next_mid_point.first << " : " << next_mid_point.second << " | "
				<< "smooth: " << smoother.first << " : " << smoother.second << "\n";*/
		}
	}


}

const void CatmullClark::invertSubDivision(std::vector<std::pair<double, double>>& coords) {
	std::vector<std::pair<double, double>> coords_backup(coords);
	coords.clear();


	for (size_t i = 0; i < coords_backup.size(); ++i) {

		if (i == 0 || i == coords_backup.size() - 1) {
			coords.push_back(coords_backup.at(i));
		}
		else if (i % 2 == 0) {
			std::pair<double, double> prev_point = coords_backup.at(i - 1);
			std::pair<double, double> current_point = coords_backup.at(i);
			std::pair<double, double> next_point = coords_backup.at(i + 1);
			std::pair<double, double> result = makeRough(current_point, prev_point, next_point);
			/*std::cout << "candidate: " << current_point.first << " : " << current_point.second << " | "
				<<"prev: " << prev_point.first << " : " << prev_point.second << " | "
				<<"next: " << next_point.first << " : " << next_point.second << " | "
				<<"result: " << result.first << " : " << result.second << " | "
				<< "\n";*/

			coords.push_back(result);
		}
	}

	/*std::cout << "new size: " << coords.size() << "\n";
	std::cout << "------BEGIN-----" << "\n";
	for (auto& t : coords) {
		std::cout << "(" << t.first << ";" << t.second << ")\n";
	}
	std::cout << "------END-----" << "\n";*/
}