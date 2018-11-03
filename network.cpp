#include "network.h"
#include "random.h"
#include <vector>
#include <map>
#include <algorithm>

void Network::resize(const size_t& new_size)
{
	std::vector<double> temp (new_size);
	RandomNumbers b;
	b.normal(temp);
	values = temp;
}

bool Network::add_link(const size_t& a, const size_t& b)
{
	if ((a > values.size()-1) or (b > values.size()-1) or (a==b))
	{
		return false;
	}
	else
	{
		std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> tab;    //! s'assure que le lien n'existe pas déjà en itérant sur toutes les cases de la map ayant la clé a (comme les liens sont bidirectionnels, pas besoin de tester avec b également)
		tab = links.equal_range(a);
		for (std::multimap<size_t, size_t>::iterator it = tab.first; it!=tab.second; ++it)
		{
			if (it->second == b)
			{
				return false;
			}
		}
	}
	links.insert(std::pair <size_t, size_t> (a, b));
	links.insert(std::pair <size_t, size_t> (b, a));		//! ajoute le lien dans les deux sens
	return true;
}

size_t Network::random_connect(const double& mean)
{
	if (!links.empty())
	{
		for (size_t i (0); i < size(); ++i)
		{
			links.erase(i);
		}	
	}					//! effacement de tous les liens existants
	
	RandomNumbers n;
	size_t num_links (0);
	for (size_t i (0); i < size(); ++i)
	{
		int nb_links (n.poisson(mean));
		while (nb_links > 0)
		{
			if (add_link(i, n.uniform_double(0, size())))   //! chaque noeud à la même probabilité d'être choisi
			{
				--nb_links;
				++num_links;
			}
		}
	}
	return num_links;			//! le nombre de liens bidirectionnels créés est retourné
}

size_t Network::set_values (const std::vector<double>& new_val)
{
	if (new_val.size() < size())
	{
		for (size_t i (0); i < new_val.size(); ++i)
		{
			values[i]=new_val[i];
		}
		return new_val.size();
	}
	else
	{
		for (size_t j (0); j < size(); ++j)
		{
			values[j]=new_val[j];
		}
		return size();
	}
}

size_t Network::size() const
{
	return values.size();
}

size_t Network::degree(const size_t& n) const
{
	return links.count(n);				//! retourne le nombre d'occurence de "n" dans la map, et donc le nombre de lien que le noeud a
}

double Network::value (const size_t& n) const
{
		return values[n];
}

std::vector<double> Network::sorted_values() const
 {
	 std::vector<double> result (values);
	if (!result.empty())							//! ne trie que si le tableau n'est pas vide, renvoie le tableau vide le cas échéant
	{
		std::sort (result.begin(), result.end());		//! trie le tableau dans l'ordre ascendant
		std::reverse(result.begin(), result.end());		//!inverse l'ordre pour avoir un classement descendant
	}
	return result;
}

std::vector<size_t> Network::neighbors (const size_t& n) const
{
	std::vector<size_t> result;
	for (auto I : links)
	{
		if (I.first == n)
		{
			result.push_back(I.second);		//! ajoute le voisin au tableau si la clé correspond au noeud d'intérêt
		}
	}
	return result;
}

	
