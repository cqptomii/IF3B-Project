-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1
-- Généré le : dim. 14 jan. 2024 à 09:21
-- Version du serveur : 10.4.28-MariaDB
-- Version de PHP : 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `capteurs`
--

-- --------------------------------------------------------

--
-- Structure de la table `hauteur`
--

CREATE TABLE `hauteur` (
  `id` int(11) NOT NULL,
  `valeur` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Structure de la table `humidite`
--

CREATE TABLE `humidite` (
  `id` int(11) NOT NULL,
  `val` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Déchargement des données de la table `humidite`
--

INSERT INTO `humidite` (`id`, `val`) VALUES
(0, 33),
(0, 33),
(0, 33),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 35),
(0, 35),
(0, 36),
(0, 36),
(0, 37),
(0, 38),
(0, 37),
(0, 36),
(0, 36),
(0, 36),
(0, 36),
(0, 37),
(0, 36),
(0, 36),
(0, 35),
(0, 35),
(0, 36),
(0, 36),
(0, 35),
(0, 35),
(0, 35),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 35),
(0, 35),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 34),
(0, 35),
(0, 36),
(0, 37),
(0, 37),
(0, 36),
(0, 36);

-- --------------------------------------------------------

--
-- Structure de la table `luminosite`
--

CREATE TABLE `luminosite` (
  `id` int(11) NOT NULL,
  `val` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Structure de la table `pression`
--

CREATE TABLE `pression` (
  `id` int(11) NOT NULL,
  `valeur` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Structure de la table `qualiteco2`
--

CREATE TABLE `qualiteco2` (
  `id` int(11) NOT NULL,
  `val` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Structure de la table `qualitetvoc`
--

CREATE TABLE `qualitetvoc` (
  `id` int(11) NOT NULL,
  `val` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Structure de la table `temp`
--

CREATE TABLE `temp` (
  `id` int(11) NOT NULL,
  `valeur` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `hauteur`
--
ALTER TABLE `hauteur`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `luminosite`
--
ALTER TABLE `luminosite`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `pression`
--
ALTER TABLE `pression`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `qualiteco2`
--
ALTER TABLE `qualiteco2`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `qualitetvoc`
--
ALTER TABLE `qualitetvoc`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `temp`
--
ALTER TABLE `temp`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `hauteur`
--
ALTER TABLE `hauteur`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `luminosite`
--
ALTER TABLE `luminosite`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `pression`
--
ALTER TABLE `pression`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `qualiteco2`
--
ALTER TABLE `qualiteco2`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `qualitetvoc`
--
ALTER TABLE `qualitetvoc`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `temp`
--
ALTER TABLE `temp`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=162;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
